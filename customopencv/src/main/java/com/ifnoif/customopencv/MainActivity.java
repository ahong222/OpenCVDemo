package com.ifnoif.customopencv;

import android.Manifest;
import android.app.Activity;
import android.app.AlertDialog;
import android.content.ContentResolver;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.drawable.BitmapDrawable;
import android.net.Uri;
import android.os.Bundle;
import android.provider.MediaStore;
import android.support.v4.app.ActivityCompat;
import android.util.Log;
import android.view.View;
import android.widget.ImageView;
import android.widget.Toast;

import com.ifnoif.jni.OpenCVHelper;

import org.opencv.android.OpenCVLoader;
import org.opencv.android.Utils;
import org.opencv.core.Mat;
import org.opencv.core.MatOfRect;
import org.opencv.core.Rect;
import org.opencv.core.Size;
import org.opencv.objdetect.CascadeClassifier;
import org.opencv.objdetect.Objdetect;

import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.util.List;

/**
 * Created by syh on 2016/11/21.
 */

public class MainActivity extends Activity implements View.OnClickListener {
    private static final String TAG = "MainActivity";
    private static final String[] sFilterArray = {"haarcascade_frontalface_default", "haarcascade_frontalface_alt", "haarcascade_frontalface_alt2"};
    private static final int REQUEST_SELECT_PICTURE = 100;
    private static final int REQUEST_SELECT_CAMER = 200;

    static {
        OpenCVLoader.initDebug();
    }

    private CascadeClassifier mCascadeClassifier;
    private Bitmap mSrcBitmap;

    private ImageView mSrcImageView;
    private ImageView mResultImageView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.main_activity);
        findViewById(R.id.choose).setOnClickListener(this);
        findViewById(R.id.choose_filter).setOnClickListener(this);
        findViewById(R.id.gray).setOnClickListener(this);
        findViewById(R.id.find_face).setOnClickListener(this);

        mSrcImageView = (ImageView) findViewById(R.id.original);
        mResultImageView = (ImageView) findViewById(R.id.result);

        final ImageView imgView = new ImageView(this);
        imgView.setBackgroundColor(Color.RED);


        mSrcBitmap = ((BitmapDrawable) getResources().getDrawable(
                R.drawable.genie)).getBitmap();
        mSrcImageView.setImageBitmap(mSrcBitmap);

    }


    public List<Rect> findFace(CascadeClassifier cascadeClassifier, Bitmap bitmap) {
        try {
            Mat face = new Mat();
            Utils.bitmapToMat(bitmap, face);//convert original bitmap to Mat, R G B.

            MatOfRect result = new MatOfRect();
            cascadeClassifier.detectMultiScale(face, result, 1.2, 2, 0 | Objdetect.CASCADE_FIND_BIGGEST_OBJECT, new Size(20, 20), new Size(2000, 2000));
            if (result != null) {
                List<Rect> list = result.toList();
                return list;
            }
        } catch (Exception e) {
            e.printStackTrace();
        }

        return null;
    }

    private Bitmap getBlurBitmap(Bitmap bitmap, List<Rect> faceList) {
        if (faceList == null || faceList.isEmpty()) {
            return null;
        }
        Bitmap newBitmap = Bitmap.createBitmap(bitmap.getWidth(), bitmap.getHeight(), Bitmap.Config.ARGB_8888);
        Canvas canvas = new Canvas(newBitmap);

        Bitmap blurBitmap = null;
        try {
            blurBitmap = Blur.doBlur(bitmap, true);
        } catch (OutOfMemoryError e) {
            android.widget.Toast.makeText(MainActivity.this, "OOM...", Toast.LENGTH_SHORT).show();
            return null;
        }
        if (blurBitmap == null) {
            return null;
        }
        canvas.drawBitmap(blurBitmap, 0, 0, null);

        for (Rect opencvRect : faceList) {
            android.graphics.Rect srcRect = new android.graphics.Rect(opencvRect.x, opencvRect.y, opencvRect.x + opencvRect.width, opencvRect.y + opencvRect.height);
            canvas.drawBitmap(bitmap, srcRect, srcRect, null);
        }
        return newBitmap;
    }

    private String intFilterFile(int rawId) {
        InputStream is = null;
        File cascadeDir = getDir("cascade", Context.MODE_PRIVATE);
        File mCascadeFile = new File(cascadeDir, "cascade.xml");

        FileOutputStream os = null;
        try {
            is = getResources().openRawResource(rawId);
            os = new FileOutputStream(mCascadeFile);
            byte[] buffer = new byte[4096];
            int bytesRead;
            while ((bytesRead = is.read(buffer)) != -1) {
                os.write(buffer, 0, bytesRead);
            }
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            try {
                is.close();
            } catch (Exception e) {
                e.printStackTrace();
            }
            try {
                os.close();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        return mCascadeFile.getAbsolutePath();
    }

    private CascadeClassifier createCascadeClassifier(String filePath) {
        Log.d(TAG, "createCascadeClassifier filePath:" + filePath);
        if (filePath == null) {
            return null;
        }
        CascadeClassifier cascadeClassifier = new CascadeClassifier(filePath);
        cascadeClassifier.load(filePath);
        if (cascadeClassifier.empty()) {
            Log.e(TAG, "result Failed to load cascade classifier");
            cascadeClassifier = null;
        }
        return cascadeClassifier;
    }

    private Bitmap getGrayBitmap(Bitmap bitmap) {
        try {
            int w = bitmap.getWidth(), h = bitmap.getHeight();
            int[] pix = new int[w * h];
            bitmap.getPixels(pix, 0, w, 0, 0, w, h);
            int[] resultPixes = OpenCVHelper.gray(pix, w, h);
            Bitmap result = Bitmap.createBitmap(w, h, Bitmap.Config.RGB_565);
            result.setPixels(resultPixes, 0, w, 0, 0, w, h);
            return result;
        } catch (Exception e) {
            e.printStackTrace();
        }
        return null;
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.choose: {
                showChooseImgDialog();
                break;
            }
            case R.id.choose_filter: {
                showChooseFilterDialog();
                break;
            }
            case R.id.gray: {
                if (!checkSrcBitmap()) {
                    return;
                }
                Bitmap gray = getGrayBitmap(mSrcBitmap);
                if (gray == null) {
                    onFail();
                } else {
                    onSuccess();
                    mResultImageView.setImageBitmap(gray);
                }
                break;
            }
            case R.id.find_face: {
                if (!checkSrcBitmap()) {
                    return;
                }
                if (!checkFilter()) {
                    return;
                }
                List<Rect> faceList = findFace(mCascadeClassifier, mSrcBitmap);
                Bitmap blurBitmap = getBlurBitmap(mSrcBitmap, faceList);
                if (blurBitmap == null) {
                    onFail();
                } else {
                    onSuccess();
                    mResultImageView.setImageBitmap(blurBitmap);
                }
                break;
            }
        }
    }

    private boolean checkSrcBitmap() {
        if (mSrcBitmap == null) {
            android.widget.Toast.makeText(MainActivity.this, "请选择图片", Toast.LENGTH_SHORT).show();
            return false;
        }

        return true;
    }

    private boolean checkFilter() {
        if (mCascadeClassifier == null) {
            android.widget.Toast.makeText(MainActivity.this, "请选择分类器", Toast.LENGTH_SHORT).show();
            return false;
        }
        return true;
    }

    private void onFail() {
        android.widget.Toast.makeText(MainActivity.this, "处理失败", Toast.LENGTH_SHORT).show();
    }

    private void onSuccess() {
        android.widget.Toast.makeText(MainActivity.this, "处理成功", Toast.LENGTH_SHORT).show();
    }

    private void showChooseFilterDialog() {
        AlertDialog dialog = new AlertDialog.Builder(MainActivity.this).setItems(sFilterArray, new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                Log.d(TAG, "DialogInterface onClick which:" + which);
                String filterName = sFilterArray[which];
                int rawId = getResources().getIdentifier(filterName, "raw", getPackageName());
                String file = intFilterFile(rawId);
                mCascadeClassifier = createCascadeClassifier(file);
                if (mCascadeClassifier == null) {
                    onFail();
                } else {
                    onSuccess();
                }
            }
        }).create();
        dialog.show();
    }


    private void showChooseImgDialog() {
        CharSequence[] items = {"相册", "相机"};
        new AlertDialog.Builder(this)
                .setTitle("选择图片来源")
                .setItems(items, new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int which) {
                        if (which == 0) {
                            Intent intent = new Intent(Intent.ACTION_GET_CONTENT);
                            intent.addCategory(Intent.CATEGORY_OPENABLE);
                            intent.setType("image/*");
                            startActivityForResult(Intent.createChooser(intent, "选择图片"), REQUEST_SELECT_PICTURE);
                        } else {
                            Intent intent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
                            startActivityForResult(intent, REQUEST_SELECT_CAMER);
                        }
                    }
                })
                .create().show();
    }

    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (resultCode == RESULT_OK) {
            Bitmap bitmap = null;
            if (requestCode == REQUEST_SELECT_PICTURE) {
                //选择图片
                Uri uri = data.getData();
                ContentResolver cr = this.getContentResolver();
                try {
                    bitmap = BitmapFactory.decodeStream(cr.openInputStream(uri));
                } catch (Exception e) {
                    e.printStackTrace();
                }
            } else {
                if (data.getData() != null || data.getExtras() != null) { //防止没有返回结果
                    Uri uri = data.getData();
                    if (uri != null) {
                        bitmap = BitmapFactory.decodeFile(uri.getPath()); //拿到图片
                    }
                    if (bitmap == null) {
                        Bundle bundle = data.getExtras();
                        if (bundle != null) {
                            bitmap = (Bitmap) bundle.get("data");
                        }
                    }
                }
            }

            if (bitmap != null) {
                mSrcBitmap = bitmap;
                mSrcImageView.setImageBitmap(mSrcBitmap);
                return;
            }
            onFail();

        }
    }
}
