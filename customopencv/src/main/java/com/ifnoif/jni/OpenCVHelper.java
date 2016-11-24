package com.ifnoif.jni;

import org.opencv.core.MatOfRect;
import org.opencv.core.Rect;

import org.opencv.core.Mat;
import org.opencv.core.Size;
import org.opencv.objdetect.CascadeClassifier;
import org.opencv.objdetect.Objdetect;

/**
 * Created by syh on 2016/11/21.
 */

public class OpenCVHelper {
    static {
        System.loadLibrary("OpenCV");
    }
    public static native int[] gray(int[] buf, int w, int h);


}
