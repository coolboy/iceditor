package com.pitt.softengr.icbuilder.util.event;

/**
 * A simple event object describing progress towards a goal value.
 * 
 * @author Lou Angelucci
 *
 */
public class ProgressEvent extends BuilderEvent{
 
    /** Generated serial version UID. */
    private static final long serialVersionUID = 3239366426780879416L;
 
    private int min;
    private int max;
    
    /** The progress value. */
    private int progress;
    
    private boolean validate = false;

    /**
     * Constructor.
     * @param source Where the event came from when it was created.
     * @param min The minimum number where progress is considered to be at 0%.
     * @param max The maximum number where progress is considered to be at 100%.
     * @param progress A number between min and max that describes how 
     * much progress has been completed.
     * @throws IllegalArgumentException for progress values below min or above max.
     */
    public ProgressEvent(Object source, int min, int max, int progress) {
        // Necessary because the superclass requires a source value.
        super(source);
        this.min = min;
        this.max = max;
        // Validate the progress value sent.
        if (validate) {
            if (progress < min || progress > max) {
                throw new IllegalArgumentException(
                        "Progress value out of range. (value=" + progress + 
                        ", min=" + min + ", max=" + max + ")");
            }
        }
        // Set the progress value.
        this.progress = progress;
    }
    
    public double getPercentage() {
        int scale = this.getMax() - this.getMin();
        double percent = (double) this.getProgress() / (double) scale;
        return percent;
    }
    
    public boolean isComplete() {
        return this.getMax() == this.getProgress();
    }
    
    /**
     * Gets the progress.
     * @return A value representing how far the progress is to completion.
     */
    public int getProgress() {
        return progress;
    }
    
    public int getMin() {
        return min;
    }

    public int getMax() {
        return max;
    }
}
