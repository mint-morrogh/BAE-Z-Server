/**
 * HUDClock - BAE-Z
 *
 * Displays the in-game world time (HH:MM) in the top-right corner.
 * Fixed position — stays put whether minimap is open or closed.
 */

modded class IngameHud
{
	protected TextWidget m_ClockWidget;
	protected float m_ClockTimer;

	override void Init(Widget hud_panel_widget)
	{
		super.Init(hud_panel_widget);

		// Create clock at root level for screen-space positioning
		Widget clockRoot = GetGame().GetWorkspace().CreateWidgets("HUDClock/Scripts/5_Mission/HUDClock.layout");
		if (clockRoot)
			m_ClockWidget = TextWidget.Cast(clockRoot);

		m_ClockTimer = 0;
	}

	override void Update(float timeslice)
	{
		super.Update(timeslice);

		if (!m_ClockWidget)
			return;

		// Hide when a menu is open (inventory, pause, etc.)
		UIScriptedMenu topMenu = GetGame().GetUIManager().GetMenu();
		if (topMenu)
		{
			m_ClockWidget.Show(false);
			return;
		}
		m_ClockWidget.Show(true);

		m_ClockTimer += timeslice;
		if (m_ClockTimer < 1.0)
			return;
		m_ClockTimer = 0;

		// Update time display
		int year, month, day, hour, minute;
		GetGame().GetWorld().GetDate(year, month, day, hour, minute);

		string timeStr = "";
		if (hour < 10)
			timeStr = "0";
		timeStr += hour.ToString() + ":";
		if (minute < 10)
			timeStr += "0";
		timeStr += minute.ToString();
		m_ClockWidget.SetText(timeStr);
	}
}
