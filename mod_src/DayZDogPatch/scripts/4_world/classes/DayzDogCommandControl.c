class DayzDogCommandControl : DayZAnimalCommandScript
{
	protected Dayz_Doggo m_Dog;
	protected DayzDogST m_ST;
	
	private float m_Time;
	
	private float m_PathAngle;
	private float m_PathAngleDiff;
	
	private float m_TurnSpeed;
	private float m_TurnTargetSpeed;
	private float m_MaxTurnSpeed;
	private float m_MaxTurnAcceleration;
	
	private float m_Speed;
	private float m_TargetSpeed;
	private float m_MaxSpeed;
	private float m_MaxAcceleration;
	
	private vector m_TargetPosition;
	private vector m_NextTargetPosition;
	
	private bool m_Jump;
	private bool m_Attack;
	private bool m_Attack2;
	
	void DayzDogCommandControl( DayZAnimal pAnimal )
	{
		m_Dog = Dayz_Doggo.Cast(pAnimal);
		m_ST = m_Dog.GetAnimST();
	}
	
	override void OnActivate()
	{
		m_Time = 0.0;
		m_TurnSpeed = 0.0;
		m_PathAngleDiff = 0.0;
		m_TurnTargetSpeed = 0.0;
		m_MaxTurnSpeed = 25.0; //25
		m_MaxTurnAcceleration = 1.0;
		
		m_Speed = 0.0;
		m_TargetSpeed = 0.0;
		m_MaxSpeed = 5.0;
		m_MaxAcceleration = 5.0; //4
		m_Jump = false;
		m_Attack = false;
		m_ST.SetAlertLevel(this,3);
		m_ST.SetBehavior(this, 1);
	}
	
	override void PreAnimUpdate( float pDt )
	{
		
		float speed = m_Speed; // / 0.5;
		float mspeed;
		speed = Math.Clamp( speed, 0, 5 ); //5
		m_ST.SetAlertLevel(this,3);
		m_ST.SetTurn( this, m_PathAngleDiff*1.5 );
		m_ST.SetLookXAngle(this, m_PathAngleDiff*1.5);
		// howto rotate with model after turn

		if (speed == 1)
			mspeed = 3;
		if (speed == 2)
			mspeed = 3.8;
		if (speed >= 3)
			mspeed = 5.2;
		/*
		if (speed == 1)
			mspeed = 2;
		if (speed == 2)
			mspeed = 4;
		if (speed >= 3)
			mspeed = 5;
		*/
		m_ST.SetSpeed( this, mspeed );
		if (m_Jump)
		{
			//dBodySetInteractionLayer(m_Dog, PhxInteractionLayers.BUILDING);
			//dBodySetInteractionLayer(m_Dog, PhxInteractionLayers.TERRAIN );
			m_ST.CallJump(this, 101, 0);
			
		}
		m_Jump = false;
		if (m_Attack && !m_Attack2)
			m_ST.CallAttack(this, 5, 0);
		
		if (m_Attack && m_Attack2)
			m_ST.CallAttack(this, 6, 0);
		m_Attack = false;
		m_Attack2 = false;
	}
	
	override void PrePhysUpdate( float pDt )
	{
		PlayerBase owner = GetPlayerByEntityID(m_Dog.GetOwnerId());
		HumanInputController hic;
		
		float plrspeed;
		vector plrdirection;
		
		hic = owner.GetInputController();
		hic.GetMovement(plrspeed,plrdirection);
		//HumanCommandMove hcm = owner.GetCommand_Move();
		//m_Speed = hcm.GetCurrentMovementSpeed();
		//hic.OverrideMovementSpeed(true, 0);
		//hic.OverrideAimChangeX(true, 0);
		m_Speed = plrspeed;
		if (hic.IsJumpClimb())
			m_Jump = true;
		if (hic.IsUseButton())
			m_Attack = true;
		if (hic.IsWeaponRaised())
			m_Attack2 = true;
		/*
		Input input = GetGame().GetInput();
		
		m_PathAngleDiff = input.LocalValue("UAAimLeft") - input.LocalValue("UAAimRight");
		float forward = input.LocalValue("UAMoveForward") - input.LocalValue("UAMoveBack");
		m_Speed = forward + 1;
		if( input.LocalValue("UATurbo") )
			m_Speed *= 2;

		if( input.LocalValue("UAWalkRunTemp") )
			m_Speed *= 0.5;
		
		//float pitchDiff = input.LocalValue("UAAimDown") - input.LocalValue("UAAimUp");
		vector oldOrient = m_Dog.GetOrientation();
		vector newOrient = oldOrient;
		newOrient[0] = newOrient[0] - Math.RAD2DEG * m_PathAngleDiff * pDt;
		//newOrient[1] = newOrient[1] - Math.RAD2DEG * pitchDiff * pDt;
		if( newOrient[1] < -89 )
			newOrient[1] = -89;
		if( newOrient[1] > 89 )
			newOrient[1] = 89;
		
		SetRotation(Vector( -m_PathAngleDiff*1.5, 0, 0 ));
		*/
		m_PathAngleDiff = Math.RAD2DEG*( hic.GetAimChange()[0]);
		//DoggoLogger.Log("PATH ANGLE DIFF " + m_PathAngleDiff);

		//SetRotation( Vector( m_PathAngleDiff , 0 , 0 ) );
		SetYaw( Vector( m_PathAngleDiff , 0 , 0 ) );
		//DayZAnimalInputController dc = m_Dog.GetInputController();
		//dc.OverrideTurnSpeed(true, 5);
		//dc.OverrideHeading(true, dc.GetHeading() + m_PathAngleDiff);
		//dc.OverrideHeading(true, 180);	
		
		
	}
	
	override bool PostPhysUpdate( float pDt )
	{		
		//dBodySetInteractionLayer(m_Dog, PhxInteractionLayers.RAGDOLL );		
		return true;
	}
	
	void AnglesToQuat( vector angles, out float[] quat )
	{
		vector rotationMatrix[3];
		Math3D.YawPitchRollMatrix( angles, rotationMatrix );
		Math3D.MatrixToQuat( rotationMatrix, quat );
	}
	
	void SetRotation( vector angles )
	{
		float rotation[4];
		AnglesToQuat( angles, rotation );
		PrePhys_SetRotation( rotation );
	}
	
	void SetYaw( vector angles )
	{
		float rotation[4];
		float oldrot[4];
		PrePhys_GetRotation(oldrot);
		AnglesToQuat( angles, rotation );
		rotation[0] = oldrot[0];
		rotation[2] = oldrot[2];
		rotation[3] = oldrot[3];
		PrePhys_SetRotation( rotation );
	}
	
	protected static PlayerBase GetPlayerByEntityID(int entityID) //bylo protected static
	{
		array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);
		for(int i = 0; i < players.Count(); i++)
		{
			if(players.Get(i).GetID() == entityID)
			{
				return PlayerBase.Cast(players.Get(i));
			}
		}
		return null;
	}
};