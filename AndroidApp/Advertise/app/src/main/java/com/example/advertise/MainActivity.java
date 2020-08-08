package com.example.advertise;

import androidx.appcompat.app.AppCompatActivity;

import android.content.ContentResolver;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.InetSocketAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;

public class MainActivity extends AppCompatActivity {

    private static String TAG = "GXS";

    private boolean mIsStart = false;
    private boolean mIsResume = false;
    private boolean mIsPushing = false;

    private ServerSocket mServerSocket;
    private static ArrayList<Socket> mSocketList = new ArrayList<Socket>();

    private final int MSG_SERVER_START_OK = 0;
    private final int MSG_PUSHING = 1;
    private final int MSG_PUSH_OK = 2;
    private final int MSG_PUSH_ERROR = 3;
    private final int MSG_PUSH_NO_PHOTO = 4;
    private final int MSG_CONNECT_NUM = 5;
    private final int MSG_UPGRADE_FILE_PUSHING = 6;
    private final int MSG_PUSH_UPGRADE_FILE_OK = 7;
    private final int MSG_PUSH_UPGRADE_FILE_ERROR = 8;
    private final int MSG_PUSH_NO_UPGRADE_FILE = 9;

    private ImageView mImageView;
    private TextView mPushHint;
    private TextView mServerStatus;
    private TextView mConnectNum;

    private TextView mUpgradeFileTextView;
    private TextView mPushUpgradeHintTextView;

    private Bitmap mCacheBmp;

    private Uri mImageUri;

    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.start_socketserver:
                if (!mIsStart) {
                    startSocketServer();
                }
                break;
            case R.id.add_photo:
                Intent intent = new Intent();
                /* 开启Pictures画面Type设定为image */
                intent.setType("image/*");
                /* 使用Intent.ACTION_GET_CONTENT这个Action */
                intent.setAction(Intent.ACTION_GET_CONTENT);
                /* 取得相片后返回本画面 */
                startActivityForResult(intent, 1);
                break;
            case R.id.push_photo:
                pushPhoto();
                break;
            case R.id.add_upgrade_file:
                Intent i = new Intent(Intent.ACTION_GET_CONTENT);
                i.setType("*/*");//设置类型，我这里是任意类型，任意后缀的可以这样写。
                i.addCategory(Intent.CATEGORY_OPENABLE);
                startActivityForResult(i, 2);
                break;
            case R.id.push_upgrade_file:
                pushUpgradeFile();
                break;
        }
    }

    private void startSocketServer() {
        new Thread() {
            @Override
            public void run() {
                try {
                    Log.i(TAG, "startSocketServer thread start run...");
                    mServerSocket = new ServerSocket(30000);
                    // 192.168.43.1 127.0.0.1
//                    mServerSocket.bind(new InetSocketAddress("0.0.0.0", 30000));
                    // ss.bind(new InetSocketAddress("127.0.0.1", 30000));
                    Log.i(TAG, "bind success...");

                    mIsStart = true;

                    mHandler.sendEmptyMessage(MSG_SERVER_START_OK);

                    while (mIsResume) {
                        // 此行代码会阻塞，将一直等待别人的连接
                        Socket s = mServerSocket.accept();

                        // 推送时间
                        SimpleDateFormat formatter = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
                        Date curDate = new Date(System.currentTimeMillis());	// 获取当前时间
                        String str = formatter.format(curDate);

                        DataOutputStream dos = new DataOutputStream(s.getOutputStream());
                        dos.write(str.getBytes());
                        dos.flush();

                        synchronized (mSocketList) {
                            mSocketList.add(s);
                            mHandler.sendEmptyMessage(MSG_CONNECT_NUM);
                        }
                        // 每当客户端连接后启动一条ServerThread线程为该客户端服务
                        new Thread(new ServerThread(s)).start();
                    }
                } catch (IOException e) {
                    Log.e(TAG, e.toString());
                }
            }
        }.start();
    }

    private long mPushStartTime;

    private void pushPhoto() {
        Log.i(TAG, "pushPhoto mIsStart: " + mIsStart + ", mIsPushing: " + mIsPushing
                + ", System.currentTimeMillis: " + System.currentTimeMillis() + ", mPushStartTime: " + mPushStartTime);
        if (mIsStart && !mIsPushing && (System.currentTimeMillis() - mPushStartTime > 3000)) {
            new Thread() {
                @Override
                public void run() {
                    if (mCacheBmp != null) {
                        mPushStartTime = System.currentTimeMillis();
                        mIsPushing = true;

                        mHandler.sendEmptyMessage(MSG_PUSHING);

                        // 一次最多发送4KB
                        final int BUFFER_SIZE = 4 * 1024;
                        byte[] data = new byte[BUFFER_SIZE];

                        try {
                            Log.i(TAG, "pushPhoto mSocketList size: " + mSocketList.size());
                            synchronized (mSocketList) {
                                for (Socket s : mSocketList) {
									/*// 向client发送消息
									DataOutputStream dos = new DataOutputStream(s.getOutputStream());
									ContentResolver cr = MainActivity.this.getContentResolver();
	//								InputStream fis = cr.openInputStream(mImageUri);

									// 压缩图片
									Bitmap image = comp(BitmapFactory.decodeStream(cr.openInputStream(mImageUri)));
									ByteArrayOutputStream baos = new ByteArrayOutputStream();
									image.compress(Bitmap.CompressFormat.JPEG, 100, baos);
									ByteArrayInputStream fis = new ByteArrayInputStream(baos.toByteArray());
									baos.close();

									int size = fis.available();

									Log.i(TAG, "size = " + size);
									byte[] data = new byte[size];
									fis.read(data);

									dos.writeInt(size);
									dos.write(data);

									dos.flush();
									// dos.close();
									fis.close();*/


                                    // 向client发送消息
                                    DataOutputStream dos = new DataOutputStream(s.getOutputStream());

                                    // 压缩图片
                                    ByteArrayOutputStream baos = new ByteArrayOutputStream();
                                    mCacheBmp.compress(Bitmap.CompressFormat.JPEG, 100, baos);
                                    ByteArrayInputStream fis = new ByteArrayInputStream(baos.toByteArray());
                                    baos.close();

                                    byte[] type = new byte[3];
                                    type[0] = 'j';
                                    type[1] = 'p';
                                    type[2] = 'g';

                                    dos.write(type);

                                    int size = fis.available();
                                    Log.i(TAG, "size = " + size);
                                    dos.write(intToByteArray(size));

                                    int readLen = 0;
                                    while (size > 0) {
                                        readLen = fis.read(data, 0, BUFFER_SIZE);
//										Log.i(TAG, "readLen: " + readLen);
                                        dos.write(data, 0, readLen);
                                        dos.flush();

                                        size -= readLen;
                                    }

                                    fis.close();
                                    Log.i(TAG, "write success!!!");
                                }
                            }

                            mHandler.sendEmptyMessage(MSG_PUSH_OK);
                        } catch (IOException e) {
                            e.printStackTrace();
                            Log.i(TAG, e.toString());
                            mHandler.sendEmptyMessage(MSG_PUSH_ERROR);
                        } finally {
                            mIsPushing = false;
                        }
                    } else {
                        mHandler.sendEmptyMessage(MSG_PUSH_NO_PHOTO);
                    }
                }
            }.start();
        }
    }

    private void pushUpgradeFile() {
        if (mIsStart && !mIsPushing && (System.currentTimeMillis() - mPushStartTime > 3000)) {
            new Thread() {
                @Override
                public void run() {
                    if (mUpgradeFileTextView.getText().length() != 0) {
                        mPushStartTime = System.currentTimeMillis();
                        mIsPushing = true;

                        mHandler.sendEmptyMessage(MSG_UPGRADE_FILE_PUSHING);

                        // 一次最多发送4KB
                        final int BUFFER_SIZE = 4 * 1024;
                        byte[] data = new byte[BUFFER_SIZE];

                        try {
                            Log.i(TAG, "pushUpgradeFile mSocketList size: " + mSocketList.size());
                            synchronized (mSocketList) {
                                for (Socket s : mSocketList) {
                                    // 向client发送消息
                                    DataOutputStream dos = new DataOutputStream(s.getOutputStream());

                                    InputStream in = new FileInputStream(new File(mUpgradeFileTextView.getText().toString()));

                                    byte[] type = new byte[3];
                                    type[0] = 'i';
                                    type[1] = 'm';
                                    type[2] = 'g';

                                    dos.write(type);

                                    int size = in.available();
                                    Log.i(TAG, "size = " + size);
                                    dos.write(intToByteArray(size));

                                    int readLen = 0;
                                    while (size > 0) {
                                        readLen = in.read(data, 0, BUFFER_SIZE);
//										Log.i(TAG, "readLen: " + readLen);
                                        dos.write(data, 0, readLen);
                                        dos.flush();

                                        size -= readLen;
                                    }

                                    in.close();
                                    Log.i(TAG, "write success!!!");
                                }
                            }

                            mHandler.sendEmptyMessage(MSG_PUSH_UPGRADE_FILE_OK);
                        } catch (IOException e) {
                            e.printStackTrace();
                            Log.i(TAG, e.toString());
                            mHandler.sendEmptyMessage(MSG_PUSH_UPGRADE_FILE_ERROR);
                        } finally {
                            mIsPushing = false;
                        }
                    } else {
                        mHandler.sendEmptyMessage(MSG_PUSH_NO_UPGRADE_FILE);
                    }
                }
            }.start();
        }
    }

    private static byte[] intToByteArray(int i) {
        byte[] result = new byte[4];
        result[3] = (byte) ((i >> 24) & 0xFF);
        result[2] = (byte) ((i >> 16) & 0xFF);
        result[1] = (byte) ((i >> 8) & 0xFF);
        result[0] = (byte) (i & 0xFF);

        Log.i(TAG, "intToByteArray " + Integer.toHexString(result[0]) + " " + Integer.toHexString(result[1]) + " "
                + Integer.toHexString(result[2]) + " " + Integer.toHexString(result[3]));

        return result;
    }

    @Override
    protected void onResume() {
        super.onResume();
        mIsResume = true;
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        mIsResume = false;

        synchronized (mSocketList) {
            if (mServerSocket != null) {
                try {
                    mSocketList.clear();
                    mServerSocket.close();
                    mServerSocket = null;
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    private Handler mHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
                case MSG_SERVER_START_OK:
                    mServerStatus.setText("服务器已启动");
                    break;
                case MSG_PUSHING:
                    mPushHint.setText("图片推送中...");
                    break;
                case MSG_PUSH_OK:
                    mPushHint.setText("图片推送成功!");
                    break;
                case MSG_PUSH_ERROR:
                    mPushHint.setText("图片推送失败!");
                    break;
                case MSG_PUSH_NO_PHOTO:
                    mPushHint.setText("请先选择1张图片，再点击推送按钮");
                    break;
                case MSG_CONNECT_NUM:
                    mConnectNum.setText(String.format("当前%d个设备连接", mSocketList.size()));
                    break;
                case MSG_UPGRADE_FILE_PUSHING:
                    mPushUpgradeHintTextView.setText("升级包推送中...");
                    break;
                case MSG_PUSH_UPGRADE_FILE_OK:
                    mPushUpgradeHintTextView.setText("升级包推送成功!");
                    break;
                case MSG_PUSH_UPGRADE_FILE_ERROR:
                    mPushUpgradeHintTextView.setText("升级包推送失败!");
                    break;
                case MSG_PUSH_NO_UPGRADE_FILE:
                    mPushUpgradeHintTextView.setText("请先1个升级包，再点击推送按钮");
                    break;
            }
        }
    };

    private class ServerThread implements Runnable {
        private Socket mSocket;

        public ServerThread(Socket s) {
            mSocket = s;
        }

        @Override
        public void run() {
            Log.i(TAG, "new ServerThread start run...");
            try {
                while (true) {
                    mSocket.sendUrgentData(0);
                    try {
                        Thread.sleep(1000);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            } catch (IOException e) {
                Log.e(TAG, "ServerThread " + e.toString());
            } finally {
                synchronized (mSocketList) {
                    mSocketList.remove(mSocket);
                    mHandler.sendEmptyMessage(MSG_CONNECT_NUM);
                }

                try {
                    Log.e(TAG, "close client socket!");
                    mSocket.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        initView();
    }

    private void initView() {
        mImageView = (ImageView) findViewById(R.id.photo);
        mPushHint = (TextView) findViewById(R.id.push_hint);
        mServerStatus = (TextView) findViewById(R.id.server_status);
        mConnectNum = (TextView) findViewById(R.id.connect_num);
        mUpgradeFileTextView = (TextView) findViewById(R.id.upgrade_file_path);
        mPushUpgradeHintTextView = (TextView) findViewById(R.id.push_upgrade_hint);
    }

    // 获取本地图片
    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (resultCode == RESULT_OK) {
            if (requestCode == 1) {
                mImageUri = data.getData();
                ContentResolver cr = getContentResolver();
                try {
                    Bitmap bitmap = BitmapFactory.decodeStream(cr.openInputStream(mImageUri));
                    /* 将Bitmap设定到ImageView */
                    mImageView.setDrawingCacheEnabled(true);
                    mImageView.setImageBitmap(bitmap);
                    mCacheBmp = Bitmap.createBitmap(mImageView.getDrawingCache());
                    mImageView.setDrawingCacheEnabled(false);
                } catch (FileNotFoundException e) {
                    Log.e(TAG, e.getMessage(), e);
                }
            } else if (requestCode == 2) {
                Uri uri = data.getData();
                mUpgradeFileTextView.setText(uri.getPath().toString());
            }
        }

        super.onActivityResult(requestCode, resultCode, data);
    }

    /**
     * 图片保存测试
     * @param bmp
     * @return
     */
    private static boolean saveImage(Bitmap bmp) {
        File appDir = new File("/storage/emulated/0/Download/");
        if (!appDir.exists()) {
            appDir.mkdir();
        }
        String fileName = System.currentTimeMillis() + ".jpg";
        File file = new File(appDir, fileName);
        try {
            FileOutputStream fos = new FileOutputStream(file);
            bmp.compress(Bitmap.CompressFormat.JPEG, 100, fos);
            fos.flush();
            fos.close();
            return true;
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }

        return false;
    }

    private Bitmap compressImage(Bitmap image) {
        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        image.compress(Bitmap.CompressFormat.JPEG, 100, baos);// 质量压缩方法，这里100表示不压缩，把压缩后的数据存放到baos中
        int options = 100;
        while (baos.toByteArray().length / 1024 > 100) { // 循环判断如果压缩后图片是否大于100kb,大于继续压缩
            baos.reset();// 重置baos即清空baos
            image.compress(Bitmap.CompressFormat.JPEG, options, baos);// 这里压缩options%，把压缩后的数据存放到baos中
            options -= 10;// 每次都减少10
        }
        ByteArrayInputStream isBm = new ByteArrayInputStream(baos.toByteArray());// 把压缩后的数据baos存放到ByteArrayInputStream中
        Bitmap bitmap = BitmapFactory.decodeStream(isBm, null, null);// 把ByteArrayInputStream数据生成图片

        return bitmap;
    }

    private Bitmap comp(Bitmap image) {
        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        image.compress(Bitmap.CompressFormat.JPEG, 100, baos);
        if (baos.toByteArray().length / 500 > 1024) {// 判断如果图片大于500KB,进行压缩避免在生成图片（BitmapFactory.decodeStream）时溢出
            baos.reset();	// 重置baos即清空baos
            image.compress(Bitmap.CompressFormat.JPEG, 50, baos);// 这里压缩50%，把压缩后的数据存放到baos中
        }
        ByteArrayInputStream isBm = new ByteArrayInputStream(baos.toByteArray());
        BitmapFactory.Options newOpts = new BitmapFactory.Options();
        // 开始读入图片，此时把options.inJustDecodeBounds 设回true了
        newOpts.inJustDecodeBounds = true;
        Bitmap bitmap = BitmapFactory.decodeStream(isBm, null, newOpts);
        newOpts.inJustDecodeBounds = false;
        int w = newOpts.outWidth;
        int h = newOpts.outHeight;
        // 现在主流手机比较多是800*480分辨率，所以高和宽我们设置为
        float hh = 800f;// 这里设置高度为800f
        float ww = 480f;// 这里设置宽度为480f
        // 缩放比。由于是固定比例缩放，只用高或者宽其中一个数据进行计算即可
        int be = 1;// be=1表示不缩放
        if (w > h && w > ww) {// 如果宽度大的话根据宽度固定大小缩放
            be = (int) (newOpts.outWidth / ww);
        } else if (w < h && h > hh) {// 如果高度高的话根据宽度固定大小缩放
            be = (int) (newOpts.outHeight / hh);
        }
        if (be <= 0) {
            be = 1;
        }
        newOpts.inSampleSize = be;// 设置缩放比例
        // 重新读入图片，注意此时已经把options.inJustDecodeBounds 设回false了
        isBm = new ByteArrayInputStream(baos.toByteArray());
        bitmap = BitmapFactory.decodeStream(isBm, null, newOpts);

        return compressImage(bitmap);// 压缩好比例大小后再进行质量压缩
    }
}
