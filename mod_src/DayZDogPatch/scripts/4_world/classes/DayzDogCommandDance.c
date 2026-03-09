class DayzDogCommandDance : DayZAnimalCommandScript
{
	private Dayz_Doggo m_Dog;
	private ref DayzDogST m_ST;
	
	private bool m_Sit;
	private bool m_WasSit;

	void DayzDogCommandDance( DayZAnimal pAnimal )
	{
		m_Dog = Dayz_Doggo.Cast(GetEntity());
		m_ST = m_Dog.GetAnimST();		
	}

	void ~DayzDogCommandDance()
	{
	}

	override void OnActivate()
	{
		m_Sit = false;
		m_WasSit = true;
		//m_ST.SetBehavior(this, 1);
	}

	override void OnDeactivate()
	{
		
	}

	override void PreAnimUpdate( float pDt )
	{		

		m_ST.SetBehavior(this, 11);
		if ( m_Sit != m_WasSit )
		{
			if ( m_Sit )
			{
				m_ST.SetBehavior(this, 11); //4 10
			} else
			{
				m_ST.SetBehavior(this, 1); //6 pro jidlo //1
			}
			
			m_WasSit = m_Sit;
		}
		
		
		m_ST.SetTurn( this, 0 );
		m_ST.SetSpeed( this, 0 );
	}

	override void PrePhysUpdate( float pDt )
	{
		SetRotation( vector.Zero );
		PrePhys_SetTranslation( vector.Zero );
		
		
		PlayerBase player = GetPlayerByEntityID(m_Dog.GetOwnerId());
		if (player)
		{
			if ( vector.Distance(m_Dog.GetPosition(), player.GetPosition()) <= 1.45 && player.IsEmotePlaying() )
			{
				m_Sit = true;	
			}
			else
			{
				m_Sit = false;
			}
		}
		return;
		
	}

	override bool PostPhysUpdate( float pDt )
	{		
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