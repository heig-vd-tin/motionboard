#define __regulc__

#include "device.h"
#include "memory.h"
#include "common.h"
#include "regul.h"
#include "IQmathLib.h"

extern volatile globalConfiguration global;

#define PSAT  32767
#define NSAT -32767

void
InitControllersA(void)
{
  bldc_a.currents.gi_d = global.mca.tq_gi;
  bldc_a.currents.kp_d = global.mca.tq_kp;
  bldc_a.currents.gi_q = global.mca.tq_gi;
  bldc_a.currents.kp_q = global.mca.tq_kp;
  bldc_a.velocity.kp = (long)global.mca.sp_kp<<4;
  bldc_a.velocity.gi = global.mca.sp_gi;
  bldc_a.position.kp = (long)global.mca.pos_kp<<2;
  bldc_a.position.gi = global.mca.pos_gi;

  bldc_a.currents.iaction_d = 0;
  bldc_a.currents.iaction_q = 0;
  bldc_a.velocity.iaction = 0;
  bldc_a.position.iaction = 0;
}

void
InitControllersB(void)
{
  bldc_b.currents.gi_d = global.mcb.tq_gi;
  bldc_b.currents.kp_d = global.mcb.tq_kp;
  bldc_b.currents.gi_q = global.mcb.tq_gi;
  bldc_b.currents.kp_q = global.mcb.tq_kp;
  bldc_b.velocity.kp = (long)global.mcb.sp_kp<<4;
  bldc_b.velocity.gi = global.mcb.sp_gi;
  bldc_b.position.kp = (long)global.mcb.pos_kp<<2;
  bldc_b.position.gi = global.mcb.pos_gi;

  bldc_b.currents.iaction_d = 0;
  bldc_b.currents.iaction_q = 0;
  bldc_b.velocity.iaction = 0;
  bldc_b.position.iaction = 0;
}

void
InitControllers (void)
{
  InitControllersA();
  InitControllersB();
}

/**
 * Regulateur PI générique
 * @param y : Grandeur mesurée
 * @param w : Valeur de consigne
 * @param kp : Gain proportionnel
 * @param gi : Gain intégral
 * @param iaction : buffer intégral (ne pas oublier d're-initialiser)
 * @return Grandeur réglée
 */
#if 0
int RegulPid (_iq y, _iq w, handle_controller_pid * h)
{
  _iq e, paction, u, daction;

  // Error
  e = w-y;

  // Integral Action
  h->iaction += _IQmpy(h->gi, e);
  h->iaction = _IQsat((h->iaction), PSAT, NSAT);

  // Proportional Action
  paction = _IQmpy(e, h->kp);

  // Differential Action
  daction = _IQmpy(e - h->e_old, h->td);

  // Result
  u = _IQsat((h->iaction) + paction + daction, PSAT, NSAT);

  return (int)u;
}
#endif

/**
 * Regulateurs de courant
 * @param ivect : Vecteur dq des courants mesurés {[1.15],[1.15]}
 * @param torque : Couple de consigne [1.15]
 * @param bldc : Structure de données du moteur réglé
 * @return Vectque dq de commande de tension {[1.15],[1.15]}
 */
vectors_phases
RegulPi_i (vectors_phases ivect, int torque, bldc_controller *bldc)
{
  vectors_phases uvect;
  uvect.d = RegulPi ((int)ivect.d, 0,      (int)bldc->currents.kp_d, bldc->currents.gi_d, &(bldc->currents.iaction_d));
  uvect.q = RegulPi ((int)ivect.q, torque, (int)bldc->currents.kp_q, bldc->currents.gi_q, &(bldc->currents.iaction_q));
  return uvect;
}

/**
 * Regulateurs de vitesse
 * @param y_velocity : Grandeur de vitesse mesurée [1.15]
 * @param w_velocity : Consigne de vitesse [1.15]
 * @param bldc : Structure de données du moteur réglé
 * @return Grandeur réglée [1.15]
 */
int
RegulPi_v (int y_velocity, int w_velocity, bldc_controller *bldc)
{
  int speed;
  speed = RegulPi ((int)y_velocity, (int)w_velocity, bldc->velocity.kp, bldc->velocity.gi, &(bldc->velocity.iaction));
  return speed;
}

/**
 * Regulateurs de position
 * @param y_position : Grandeur de position mesurée [1.15]
 * @param w_position : Consigne de position [1.15]
 * @param bldc : Structure de données du moteur réglé
 * @return Grandeur réglée [1.15]
 */
int
RegulPi_p (long y_position, long w_position, bldc_controller *bldc)
{
  int position;
  position = RegulPi (y_position, w_position, bldc->position.kp, bldc->position.gi, &(bldc->position.iaction));
  return position;
}

/**
 * Gestion des limites.
 */
int amin, amax;
_iq tmp;
_iq scale;

_iq ala = _IQ(1.0/1.732);


three_phases
Limits (three_phases u)
{
  int min, max, offset;
  _iq uu, uv, uw;

  // Détermine la grandeur la plus grande et la plus petite
  if (u.u < u.v)
  {
    max = u.v;
    min = u.u;
  }
  else
  {
    max = u.u;
    min = u.v;
  }
  if (max < u.w)  max = u.w;
  if (min > u.w)  min = u.w;

  // Correction d'offset
  offset = (max/2 + min/2);
  uu = u.u - offset;
  uv = u.v - offset;
  uw = u.w - offset;

  // Correction de gain
  tmp = (long)max - (long)min;
#if 0
  if (tmp > (2L*PSAT))
    scale = _IQdiv((2L*PSAT)-1,tmp);
  else

  scale = _IQ(2.0/1.725); // sqrt(3)/2 un peu diminué

  u.u = _IQmpy(uu, scale);
  u.v = _IQmpy(uv, scale);
  u.w = _IQmpy(uw, scale);
#endif

u.u = uu;
u.v = uv;
u.w = uw;

  return u;
}

// Calcul I2t
#if 0
2T_Process (vectors_phases i)
{
  _iq id, iq, i2t;

  id = _IQmpy(i.d, invIStall);
  iq = _IQmpy(i.q, invIStall);

  id = _IQmpy(id, id);
  iq = _IQmpy(iq, iq);

  i2t = id + iq;
}
#endif
