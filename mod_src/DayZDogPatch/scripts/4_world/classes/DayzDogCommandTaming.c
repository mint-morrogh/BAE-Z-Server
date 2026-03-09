class DayzDogCommandTaming : DayZAnimalCommandScript
{
	private Dayz_Doggo m_Dog;
	private ref DayzDogST m_ST;
	private bool m_Taming;
	private bool m_WasTamed;

	void DayzDogCommandTaming( DayZAnimal pAnimal )
	{
		m_Dog = Dayz_Doggo.Cast(GetEntity());
		m_ST = m_Dog.GetAnimST();
	}

	void ~DayzDogCommandTaming()
	{
	}

	override void OnActivate()
	{
		m_Taming = true;
		m_WasTamed = false;
	}

	override void OnDeactivate()
	{

	}

	override void PreAnimUpdate( float pDt )
	{		

		if (m_Taming != m_WasTamed)
		{
			if (m_Taming)
			{
				m_ST.SetBehavior(this, 12);
			}
			else
			{
				m_ST.SetBehavior(this, 0); //0
			}
			m_WasTamed = m_Taming;
		}
		
		
		m_ST.SetTurn( this, 0 );
		m_ST.SetSpeed( this, 0 );
	}

	override void PrePhysUpdate( float pDt )
	{
		SetRotation( vector.Zero );
		PrePhys_SetTranslation( vector.Zero );
				
		if (m_Dog.GetDoggoCmd() == DoggoCmd.TAME)
		{
			m_Taming = true;
		}
		else
		{
			m_Taming = false;
		}
			
		return;
		
	}

	override bool PostPhysUpdate( float pDt )
	{		
		if (m_Taming)
		{
			m_ST.SetBehavior(this, 12);
		}
		else
		{
			m_ST.SetBehavior(this, 1);
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
};