/**
 * MinimapTweak - BAE-Z
 *
 * - Moves minimap to top-right corner
 * - Hides coordinate/stats overlay on the minimap HUD
 * - Fixes player arrow disappearing after Tab (inventory)
 *
 * Loads AFTER ExpansionGPS_Override via requiredAddons.
 */

modded class IngameHud
{
	override void Init(Widget hud_panel_widget)
	{
		super.Init(hud_panel_widget);

		if (!m_GPSPanel)
			return;

		// Reposition minimap to top-right corner.
		// Layout uses halign right_ref + valign bottom_ref,
		// so Y increases upward from screen bottom.
		// Panel height is ~0.21, so Y=0.77 puts the top at ~98% (2% margin).
		float px, py;
		m_GPSPanel.GetPos(px, py);
		m_GPSPanel.SetPos(px, 0.77);
	}

	override void RefreshHudVisibility()
	{
		super.RefreshHudVisibility();

		// Force-hide coords/stats on the minimap every HUD refresh
		if (m_MapStatsPanel)
			m_MapStatsPanel.Show(false);
	}

	override void Update(float timeslice)
	{
		super.Update(timeslice);

		// Fix: Expansion hides the player arrow when a menu opens (Tab)
		// but a bad visibility check prevents it from coming back.
		// Re-show the arrow whenever the minimap is active and no menu is open.
		if (m_PlayerArrowMarker && m_ExpansionGPSState && m_ExpansionGPSMapState)
		{
			UIScriptedMenu topMenu = g_Game.GetUIManager().GetMenu();
			if (!topMenu)
				m_PlayerArrowMarker.Show(true);
		}
	}
}
