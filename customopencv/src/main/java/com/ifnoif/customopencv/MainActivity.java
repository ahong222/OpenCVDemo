package com.ifnoif.customopencv;

import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.Color;
import android.graphics.drawable.BitmapDrawable;
import android.os.Bundle;
import android.widget.Button;
import android.widget.ImageView;

import com.ifnoif.jni.OpenCVHelper;

/**
 * Created by syh on 2016/11/21.
 */

public class MainActivity extends Activity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        ImageView imgView = new ImageView(this);
        imgView.setBackgroundColor(Color.RED);


        Bitmap bitmap = ((BitmapDrawable) getResources().getDrawable(
                R.drawable.genie)).getBitmap();
        int w = bitmap.getWidth(), h = bitmap.getHeight();
        int[] pix = new int[w * h];
        bitmap.getPixels(pix, 0, w, 0, 0, w, h);
        int [] resultPixes= OpenCVHelper.gray(pix,w,h);
        Bitmap result = Bitmap.createBitmap(w,h, Bitmap.Config.RGB_565);
        result.setPixels(resultPixes, 0, w, 0, 0,w, h);
        imgView.setImageBitmap(result);

        setContentView(imgView);

    }
}
