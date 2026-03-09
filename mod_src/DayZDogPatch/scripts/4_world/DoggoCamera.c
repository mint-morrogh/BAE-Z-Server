class DoggoCamera extends Camera {

	float sendUpdateAcc = 0.0;
	Object camTarget;

	void DoggoCamera() {
		SetEventMask( EntityEvent.FRAME );

		camTarget = NULL;
	}
	
	void SetTarget(Object target)
	{
		camTarget = target;
	}

	override void EOnFrame(IEntity other, float timeSlice) {
		OnUpdate(timeSlice);

		if (sendUpdateAcc > 0.8) {
			GetGame().UpdateSpectatorPosition(GetPosition());

			sendUpdateAcc = 0;
		}

		sendUpdateAcc = sendUpdateAcc + timeSlice;
	}

	void OnUpdate(float timeSlice) {
		/*
		Dayz_Doggo targetDog = Dayz_Doggo.Cast(camTarget);
		if (!targetDog) return;

		SetOrientation(targetDog.GetOrientation());
		SetPosition(targetDog.GetPosition());
		*/
		Dayz_Doggo targetDog = Dayz_Doggo.Cast(camTarget);
		if (!targetDog) return;
		Input input = GetGame().GetInput();

		//SetOrientation(targetDog.GetOrientation());
		SetPosition(targetDog.ModelToWorld("0 1 -1"));  //wolf
		//SetPosition(targetDog.ModelToWorld("0 0.5 -0.5")); //chicken
		//SetPosition(targetDog.ModelToWorld("0 3 -3")); //bear
		
		//float yawDiff = input.LocalValue("UAAimLeft") - input.LocalValue("UAAimRight");
		float pitchDiff = input.LocalValue("UAAimDown") - input.LocalValue("UAAimUp");
		vector oldOrient = GetOrientation();
		vector newOrient = oldOrient;
		newOrient[0] = targetDog.GetOrientation()[0];
		//newOrient[0] = newOrient[0] - Math.RAD2DEG * yawDiff * timeSlice;
		newOrient[1] = newOrient[1] - Math.RAD2DEG * pitchDiff * timeSlice;
		if( newOrient[1] < -89 )
			newOrient[1] = -89;
		if( newOrient[1] > 89 )
			newOrient[1] = 89;

		SetOrientation(newOrient);
	}
}
