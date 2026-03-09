class DayzDogCommandMovement : DayZAnimalCommandScript
{
	private Dayz_Doggo m_Dog;
	private ref DayzDogST m_ST;

	private ref PGFilter m_PathFilter;

	private float m_Time;
	
	private bool m_Sit;
	private bool m_WasSit;
	
	private float m_PathAngle;
	
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

	void DayzDogCommandMovement( DayZAnimal pAnimal )
	{
		m_Dog = Dayz_Doggo.Cast(GetEntity());
		m_ST = m_Dog.GetAnimST();;

		m_PathFilter = new PGFilter();

		int inFlags = PGPolyFlags.WALK | PGPolyFlags.DOOR | PGPolyFlags.INSIDE | PGPolyFlags.JUMP_OVER | PGPolyFlags.SWIM | PGPolyFlags.SWIM_SEA ;
		int exFlags = PGPolyFlags.DISABLED | PGPolyFlags.JUMP | PGPolyFlags.CLIMB | PGPolyFlags.CRAWL | PGPolyFlags.CROUCH;

		m_PathFilter.SetFlags( inFlags, exFlags, PGPolyFlags.NONE );
		m_PathFilter.SetCost( PGAreaType.JUMP, 0.0 );
		m_PathFilter.SetCost( PGAreaType.FENCE_WALL, 0.0 );
		m_PathFilter.SetCost( PGAreaType.WATER, 1.0 );
		m_PathFilter.SetCost( PGAreaType.WATER_DEEP, 1.0);
		m_PathFilter.SetCost( PGAreaType.WATER_SEA, 1.0);
		m_PathFilter.SetCost( PGAreaType.WATER_SEA_DEEP, 1.0);
	}

	void ~DayzDogCommandMovement()
	{
		delete m_PathFilter;
	}

	override void OnActivate()
	{
		m_Time = 0.0;
		m_TurnSpeed = 0.0;
		m_TurnTargetSpeed = 0.0;
		m_MaxTurnSpeed = 25.0;
		m_MaxTurnAcceleration = 1.0;
		
		m_Speed = 0.0;
		m_TargetSpeed = 0.0;
		m_MaxSpeed = 2.9;
		m_MaxAcceleration = 4.0;
		
		m_Sit = false;
		m_WasSit = true;
	}

	override void OnDeactivate()
	{
		m_ST.SetSwiming( this, false );
	}

	override void PreAnimUpdate( float pDt )
	{		

		//float speed = m_Speed / 0.5;
		//speed = Math.Clamp( speed, 0, 5 ); //5
		m_ST.SetBehavior(this, 0);
		m_ST.SetTurn( this, 0 );
		m_ST.SetSpeed( this, 2.9 );
		if (GetGame().GetWaterDepth( m_Dog.GetPosition()) > 0.6)
		{
			m_ST.SetSwiming( this, true );
		}
		else
		{
			m_ST.SetSwiming( this, false );
		}
		//m_ST.SetTurn( this, m_TurnSpeed );
		//m_ST.SetSpeed( this, speed );
	}
	
	float ShortestAngle( float a, float b )
	{
		int aa = a;
		int bb = b;
		
		int phi = Math.AbsInt(aa - bb) % 360; 
		if ( phi > 180 )
			phi = 360 - phi;
		
		if ( ( a - b >= 0 && a - b <= 180 ) || ( a - b <=-180 && a- b>= -360 ) )
		{
			return phi;
		}
		
		return -phi;
	} 

	override void PrePhysUpdate( float pDt )
	{
		//
		
		//SetRotation( vector.Zero );
		//PrePhys_SetTranslation( vector.Zero );
		//waypoint psa	
		
		vector destination = m_Dog.GetDoggoWaypoint();

		m_NextTargetPosition = destination; //GetPosition()
		
		if ( vector.Distance( m_TargetPosition, m_NextTargetPosition ) > 0.5 || m_Dog.PathBlocked( m_Dog.GetPosition(), m_TargetPosition ) )
		{
			m_TargetPosition = m_NextTargetPosition;

			m_Dog.UpdatePath( m_TargetPosition, m_PathFilter );
		}

		m_Time += pDt;
		
		if ( m_Dog.PathCount() == 0 )
		{
			SetRotation( vector.Zero );
			//PrePhys_SetTranslation( vector.Zero );
			
			return;
		}
		
		//vector transform[4];
		//m_Dog.GetTransform(transform);
			
		vector movementDirection = "0 0 1";
		
		vector wayPoint = destination;
		int wayPointCount = m_Dog.PathCount();
		int wayPointIndex = wayPointCount - 1;
		float wayPointDistance;
		
		if ( wayPointCount >= 2 )
		{
			wayPointIndex = m_Dog.FindNext( m_Dog.GetPosition(), wayPointDistance );
			wayPoint = m_Dog.PathGet( wayPointIndex );	
		}
		
		wayPointDistance = vector.Distance( m_Dog.GetPosition(), wayPoint );
		
		m_PathAngle = m_Dog.AngleBetweenPoints( m_Dog.GetPosition(), wayPoint );
		float currentYaw = m_Dog.GetOrientation()[0];
		if ( m_PathAngle > 180.0 )
			m_PathAngle = m_PathAngle - 360.0;
		float pathAngleDiff = ShortestAngle( m_PathAngle, currentYaw );
				
		m_TurnTargetSpeed = Math.Clamp( pathAngleDiff, -m_MaxTurnSpeed, m_MaxTurnSpeed );
		
		m_MaxTurnAcceleration = 10.0;
		
		float turnAcceleration = Math.Clamp( ( m_TurnTargetSpeed - m_TurnSpeed ), -m_MaxTurnAcceleration, m_MaxTurnAcceleration ) * pDt;
		m_TurnSpeed = Math.Clamp( m_TurnSpeed + turnAcceleration, -m_MaxTurnSpeed, m_MaxTurnSpeed );
		
		/*
		if ( wayPointIndex + 1 >= wayPointCount )
		{
			if ( m_Sit || m_ST.PlayingActions( this ) )
			{
				m_Sit = wayPointDistance < 5.0;  // 5
			} else
			{
				m_Sit = wayPointDistance < 1.0; // 1
			}
		} else
		{
			m_Sit = false;
		}
		
		if ( m_Sit || m_ST.PlayingActions( this ) )
		{
			SetRotation( vector.Zero );
			PrePhys_SetTranslation( vector.Zero );
			
			return;
		}
		*/
		
		float angleDt = m_TurnSpeed * pDt;
		
		if ( Math.AbsFloat( pathAngleDiff ) > 25.0 )
		{
			m_TargetSpeed = 0.0;
			
			angleDt *= 5.0;
		} else
		{
			m_TargetSpeed =  0.0; //m_MaxSpeed;
			
			angleDt *= 0.5; //0.5
		}
		m_MaxSpeed = 2.9;//0.50;
		
		float acceleration = Math.Clamp( ( m_TargetSpeed - m_Speed ), -m_MaxSpeed, m_MaxAcceleration ) * pDt;
		m_Speed = Math.Clamp( m_Speed + acceleration, 0.0, m_MaxSpeed );
		
		SetRotation( Vector( angleDt, 0, 0 ) );
		//PrePhys_SetTranslation( movementDirection * m_Speed * pDt );
		
	}

	override bool PostPhysUpdate( float pDt )
	{		
		return true; //true
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
};