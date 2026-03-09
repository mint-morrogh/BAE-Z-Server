class DayzDogCommandEating : DayZAnimalCommandScript
{
	private Dayz_Doggo m_Dog;
	private ref DayzDogST m_ST;
	private bool m_Eating;
	private bool m_WasEat;
	protected vector m_DoggoPosition;

	void DayzDogCommandEating( DayZAnimal pAnimal )
	{
		m_Dog = Dayz_Doggo.Cast(GetEntity());
		m_ST = m_Dog.GetAnimST();
		//m_ST = st;
	}

	void ~DayzDogCommandEating()
	{
	}

	override void OnActivate()
	{
		m_Eating = false;
		m_WasEat = true;
		//m_ST.SetAlertLevel( this, 1 );
		m_ST.SetSpeed( this, 0 );
		//m_ST.SetBehavior(this, 6);
		//DoggoLogger.Log("EATING ACTIVATE");
		m_DoggoPosition = m_Dog.GetPosition();
	
	}

	override void OnDeactivate()
	{
		//m_Dog.SetEating(false);
	}

	override void PreAnimUpdate( float pDt )
	{		
		//m_ST.SetAlertLevel( this, 2 );
		m_ST.SetSpeed( this, 0 );
		m_ST.SetBehavior(this, 6);
		m_ST.SetTurn( this, 0 );
		
		
	}

	override void PrePhysUpdate( float pDt )
	{
		SetRotation( vector.Zero );
		PrePhys_SetTranslation( vector.Zero );
		m_Dog.SetPosition(m_DoggoPosition);		
	}

	override bool PostPhysUpdate( float pDt )
	{		
		//DoggoLogger.Log("DOG CMD " + m_Dog.GetCommand_Script() );
		/*
		if (m_Dog.GetDoggoCmd() == DoggoCmd.EATING)
		{
			//m_ST.SetAlertLevel( this, 1 );
			m_ST.SetBehavior(this, 6);
		}
		else
		{
			m_ST.SetBehavior(this, 1); //0 ?
		}
		*/
		return true;
	}


	/*
	override void PreAnimUpdate( float pDt )
	{		

		if (m_Eating != m_WasEat)
		{
			if (m_Eating)
			{
				m_ST.SetBehavior(this, 6);
			}
			else
			{
				m_ST.SetBehavior(this, 0); //0
			}
			m_WasEat = m_Eating;
		}
		
		
		m_ST.SetTurn( this, 0 );
		m_ST.SetSpeed( this, 0 );
	}

	override void PrePhysUpdate( float pDt )
	{

		SetRotation( vector.Zero );
		PrePhys_SetTranslation( vector.Zero );
		m_Dog.SetPosition(m_DoggoPosition);			
		if (m_Dog.GetDoggoCmd() == DoggoCmd.EATING)
		{
			m_Eating = true;
		}
		else
		{
			m_Eating = false;
		}
			
		return;
		
	}

	override bool PostPhysUpdate( float pDt )
	{		
		if (m_Eating)
		{
			m_ST.SetBehavior(this, 6);
		}
		else
		{
			m_ST.SetBehavior(this, 0); //0
		}
		
		return true;
	}
	*/
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