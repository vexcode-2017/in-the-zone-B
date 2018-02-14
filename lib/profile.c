/**
 * profile.c - Manages motion profiling (based on encoder ticks)
 **/

#pragma systemFile

/**
 * Gets the (corrected) progress amount (decmial percentage) correcting for direction, etc.
 * @param int current The current measured value
 * @param int target The target measured value
 * @return int progress The way that has
 **/
float profileProgress(int start, int current, int target) {
    return abs((float) current - (float) start) / abs((float) target - (float) start);
}

float debugPosition;


/**
 * Follows a trapezoidal acceleration curve. Good for long moves
 * @param startTicks The tick measure at the start of the movement
 * @param targetTicks The total number of ticks to be achieved by the profile
 * @param int currentTicks The current tick measures
 * @param float dvPeriod The fraction of time for the acceleration and deceleration period
 * @return int multipler A value from 0 to 1 to multiply your PWM by
 **/
float profileTrapezoid(int startTicks, int targetTicks, int currentTicks, float dvPeriod = 1.0/3.0) {
    float position = profileProgress(startTicks, currentTicks, targetTicks);
    int accelerateUntil = (targetTicks * dvPeriod);
    int decelerateAt = targetTicks - (targetTicks * dvPeriod)

    // Acceleration Period: For dvPeriod of the motion
    if (currentTicks <= accelerateUntil) {
        return position * (1 / dvPeriod);
    // Maintain Period: For (1 - 2 dvPeriod) of the motion
    } else if (currentTicks > accelerateUntil && currentTicks < decelerateAt) {
        return 1;
    // Deceleration Period: For dvPeriod of the motion
    } else if (currentTicks >= decelerateAt) {
        return (position - 1) * -(1 / dvPeriod);
    }

    // In the strange occurance that none of the above conditions activate, eliminate the multiplier
    return 1;
}

/**
 * Follows same as profileTrapezoid, but with no acceleration curve. Good for short moves
 * @param startTicks The tick measure at the start of the movement
 * @param targetTicks The total number of ticks to be achieved by the profile
 * @param int currentTicks The current tick measures
 * @param float dvPeriod The fraction of time for the acceleration and deceleration period
 * @return int multipler A value from 0 to 1 to multiply your PWM by
 **/
float profileJerk(int startTicks, int targetTicks, int currentTicks, float dvPeriod = 1.0/2.0) {
    float position = profileProgress(startTicks, currentTicks, targetTicks);
    int decelerateAt = targetTicks - 0.5 * (targetTicks * dvPeriod)

    // Regular Period
    if (currentTicks <= decelerateAt) {
        return 1;
    // Deceleration Period: For dvPeriod of the motion
    } else if (currentTicks > decelerateAt) {
        return (position - 1) * -(1 / dvPeriod);
    }

    // In the strange occurance that none of the above conditions activate, eliminate the multiplier
    return 1;
}
