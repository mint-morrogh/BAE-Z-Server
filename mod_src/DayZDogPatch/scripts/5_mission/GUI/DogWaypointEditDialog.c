class DogWaypointEditDialog extends ScriptedWidgetEventHandler 
{
	DogMapMenu m_DogMapMenu;
	Widget m_Root;
	private EditBoxWidget	m_EditBoxName;
	private ButtonWidget         m_BtnOk;
    private ButtonWidget         m_BtnCancel;
	private ref DoggoWaypoints m_ClientWaypoints;
	private vector m_MapClickPos;
	
    void DogWaypointEditDialog(DogMapMenu menu, Widget root, vector pos)
	{
		m_DogMapMenu = menu;
		m_MapClickPos = pos;
		m_Root = GetGame().GetWorkspace().CreateWidgets( "Dayz_Dog/GUI/Layouts/Confirm.layout", root );
		m_EditBoxName = EditBoxWidget.Cast(m_Root.FindAnyWidget( "EditBoxWidget") );
		m_BtnOk = ButtonWidget.Cast( m_Root.FindAnyWidget( "btnOK" ) );
		m_BtnCancel = ButtonWidget.Cast( m_Root.FindAnyWidget( "btnCancel" ) );
		m_Root.SetHandler(this);
		m_ClientWaypoints = GetDayZGame().GetDogWayPoints();
		DayzDogWaypointsConfig.LoadConfig("waypoints.json", m_ClientWaypoints);	
    }
	
	override bool OnMouseButtonDown( Widget w, int x, int y, int button ) 
	{
		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		switch(w)
    	{
			case m_BtnOk:
			//clicked to list
			//TStringStringMap waypointy
			bool skipme = false;
			for (int i = 0; i < m_ClientWaypoints.WayPoints.Count() ; ++i)
			{
				if (m_ClientWaypoints.WayPoints.Get(i).name == m_EditBoxName.GetText())
				{
					skipme = true;
				}
			}
			if (!skipme)
			{
				m_DogMapMenu.GetMapWidget().AddUserMark(GetMapClickPos(), m_EditBoxName.GetText(), ARGB(255,255,255,0), "dayz_dog\\GUI\\Imagesets\\dog_feet_small.paa");
				string message = GetMapClickPos()[0].ToString() + " " + (GetGame().SurfaceY(GetMapClickPos()[0], GetMapClickPos()[2]) + 0.4).ToString() + " " + GetMapClickPos()[2].ToString();
				m_ClientWaypoints.WayPoints.Insert(new DoggoWaypoint(m_EditBoxName.GetText(), message));
				DayzDogWaypointsConfig.SaveConfig("waypoints.json", m_ClientWaypoints);
				m_DogMapMenu.UpdateWaypointsList();
			}
			//Print("Waypoint " + m_ClientWaypoints.WayPoints.Find(m_EditBoxName.GetText()));

			
			m_DogMapMenu.HideDialog();
			break;
			
			case m_BtnCancel:
			//clicked to list
			m_DogMapMenu.HideDialog();
			break;

    	}
        return super.OnClick(w, x, y, button);
	}
	
	void SetMapClickPos(vector pos)
	{
		m_MapClickPos = pos;	
	}
	
	vector GetMapClickPos()
	{
		return m_MapClickPos;
	}
}