class PuppyCommandStay extends DayZAnimalCommandScript
{
	protected DoggoPuppy_Base m_Puppy;
	protected PuppyST m_ST;
	protected vector m_PuppyPosition;

	void PuppyCommandStay( DayZAnimal pAnimal )
	{
		m_Puppy = DoggoPuppy_Base.Cast(pAnimal);
		m_ST = m_Puppy.GetAnimST();
	}

	void ~PuppyCommandStay()
	{
	}
	
	override void OnActivate()
	{
		m_ST.SetAlertLevel( this, 0 );
		//m_PuppyPosition = m_Puppy.GetPosition();
	}

	override void OnDeactivate()
	{

	}

	override void PreAnimUpdate( float pDt )
	{		

		//m_ST.SetBehavior(this, 12);
		m_ST.SetTurn( this, 0 );
		m_ST.SetSpeed( this, 0 );
		m_ST.SetAlertLevel( this, 0 );
	}

	override void PrePhysUpdate( float pDt )
	{
		SetRotation( vector.Zero );
		PrePhys_SetTranslation( vector.Zero );
		//m_Puppy.SetPosition(m_PuppyPosition);
		
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