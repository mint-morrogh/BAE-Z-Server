class DayzDogCommandSitting : DayZAnimalCommandScript
{
	protected Dayz_Doggo m_Dog;
	protected DayzDogST m_ST;
	
	private bool m_Sit;
	private bool m_WasSit;
	private bool m_Dance;
	protected vector m_DoggoPosition;

	void DayzDogCommandSitting( DayZAnimal pAnimal )
	{
		m_Dog = Dayz_Doggo.Cast(GetEntity());
		m_ST = m_Dog.GetAnimST();
	}

	void ~DayzDogCommandSitting()
	{
	}

	override void OnActivate()
	{
		m_Sit = true; //false
		m_WasSit = false;
		m_Dance = false;
		m_ST.SetAlertLevel( this, 0 );
		//m_ST.SetAlertLevel( this, 1 );
		//m_ST.SetBehavior(this, 1);
		m_DoggoPosition = m_Dog.GetPosition();
	}

	override void OnDeactivate()
	{
		
	}

	override void PreAnimUpdate( float pDt )
	{		

		//m_ST.SetBehavior(this, 10);
		//m_ST.SetAlertLevel( this, 1 );
		m_ST.SetAlertLevel( this, 0 );
		if ( m_Sit != m_WasSit )
		{
			if ( m_Sit )
			{
				if ( m_Dance )
				{
					m_ST.SetBehavior(this, 11); //11
				} else {
					m_ST.SetBehavior(this, 10); //4 10
				}
				
			} else
			{
				m_ST.SetBehavior(this, 1); //1
			}
			
			m_WasSit = m_Sit;
		}
		
		
		m_ST.SetTurn( this, 0 );
		m_ST.SetSpeed( this, 0 );
		//m_ST.SetAlertLevel( this, 1 );
	}

	override void PrePhysUpdate( float pDt )
	{
		SetRotation( vector.Zero );
		PrePhys_SetTranslation( vector.Zero );
		m_Dog.SetPosition(m_DoggoPosition);
		
		
		PlayerBase player = GetPlayerByEntityID(m_Dog.GetOwnerId());
		m_Sit = false;
		if (player)
		{
			if ( vector.Distance(m_Dog.GetPosition(), player.GetPosition()) <= 1.45 )
			{
				m_Sit = true;	
				if (player.IsDancing())
				{
					m_Dance = true;
				} else {
					m_Dance = false;
				}
				
			}
			else
			{
				m_Sit = false;
				m_Dance = false;
			}
		}
		return;
		
	}

	override bool PostPhysUpdate( float pDt )
	{		
		PlayerBase player = GetPlayerByEntityID(m_Dog.GetOwnerId());
		if (player)
		{
			if (player.IsDancing())
			{
				m_Dance = true;
				m_ST.SetBehavior(this, 11); //11 - dancing
			} else {
				m_Dance = false;
				m_ST.SetBehavior(this, 10);
			}
		}
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
	
	protected static PlayerBase GetPlayerByEntityID(int entityID)
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