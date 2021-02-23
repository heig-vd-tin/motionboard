
class C_PiController
{
	private: 
		float integral_action; 
	public: 
		float gi; 
		float kp;

		C_PiController(float newGi, float newKp)
		{
			Reset();
			gi = newGi; 
			kp = newKp; 
		}

		float Execute(float w, float y)
		{
			float e = w-y; 

			integral_action += e*gi; 
			if(integral_action > 1.0) integral_action = 1.0; 
			if(integral_action < -1.0) integral_action = -1.0; 

			return kp*e+integral_action;
		}

		void Reset()
		{
			integral_action = 0.0; 
		}
};

