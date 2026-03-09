class DayzDogCommandStay extends DayZAnimalCommandScript
{
	protected Dayz_Doggo m_Dog;
	protected DayzDogST m_ST;
	protected vector m_DoggoPosition;

	void DayzDogCommandStay( Dayz_Doggo pAnimal )
	{
		m_Dog = pAnimal; //Dayz_Doggo.Cast(pAnimal);
		m_ST = m_Dog.GetAnimST();
	}

	void ~DayzDogCommandStay()
	{
	}
	
	override void OnActivate()
	{
		m_ST.SetAlertLevel( this, 0 );
		m_DoggoPosition = m_Dog.GetPosition();
	}

	override void OnDeactivate()
	{

	}

	override void PreAnimUpdate( float pDt )
	{		

		m_ST.SetBehavior(this, 4);
		m_ST.SetTurn( this, 0 );
		m_ST.SetSpeed( this, 0 );
		m_ST.SetAlertLevel( this, 0 );
	}

	override void PrePhysUpdate( float pDt )
	{
		SetRotation( vector.Zero );
		PrePhys_SetTranslation( vector.Zero );
		m_Dog.SetPosition(m_DoggoPosition);
		
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
};