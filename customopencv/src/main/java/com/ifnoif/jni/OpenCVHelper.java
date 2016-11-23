package com.ifnoif.jni;

/**
 * Created by syh on 2016/11/21.
 */

public class OpenCVHelper {
    static {
        System.loadLibrary("OpenCV");
    }
    public static native int[] gray(int[] buf, int w, int h);
}
