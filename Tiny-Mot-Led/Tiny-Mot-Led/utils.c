
/*
if (!active){
	if (detect_motion(2) && time > idle_time + MIN_IDLE_TIME_MS){
		active = true;
		mot_time = time;
		// Active Initialization
		SET_NEXT_WAKE(32);
		POWER_DOWN_DIS();
	}
}

if (active && time > mot_time + ACTIVE_TIME_MS){
	active = false;
	idle_time = time;
	// Idle Initialization
	SET_NEXT_WAKE(250);
	POWER_DOWN_EN();
}

if (active){
}
*/