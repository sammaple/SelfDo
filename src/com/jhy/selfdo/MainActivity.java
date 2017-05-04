package com.jhy.selfdo;

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.io.StringWriter;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

import android.app.Activity;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.Matrix;
import android.graphics.drawable.BitmapDrawable;
import android.media.AudioManager;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.Uri;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Bundle;
import android.os.DropBoxManager;
import android.os.Environment;
import android.os.Handler;
import android.os.Message;
import android.os.StatFs;
import android.text.TextUtils;
import android.text.format.Formatter;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends Activity implements OnClickListener {

	protected static final int THREAD = 0 ;
	Button bt,button_ill;
	Button bt_datachmod, button_adb, button_mac,button_sevenup;
	TextView tx, editText_mac;

	private static final String RK3128_LED_NAME = "/sys/devices/rockchip_leds_gpio.17/led_ctl";
    private static final String RK_LED_ON = "on", RK_LED_OFF = "off";
    private static File ledFile = new File(RK3128_LED_NAME);
    
    private static final String  mPathstr      = "/sys/class/gpio_sw/P";
    private static final String  mDataName     = "/data";
    private static final String  mPullName     = "/pull";
    private static final String  mDrvLevelName = "/drv_level";
    private static final String  mMulSelName   = "/mul_sel";
    private static final String  TAG ="jhy";

    private static final String NORMAL_LED_PATH   = "/sys/class/gpio_sw/normal_led";
    private static final String STANDBY_LED_PATH  = "/sys/class/gpio_sw/standby_led";
    
	Button bt_hisiadbstart, bt_hisiadbstop,button_hisiled,button_hisi_wipe,button_himeidi_dev,button_show_notify;
	Button button_notify_one,button_clearall,btn_vendorid;
	Button button_media_info,button_bkup;
	Button button_sdcard,button_routemac;
	Button button_dropbox,button_aw_led_normal,button_aw_led_standby,button_rk_led;
	
	String rk_status=RK_LED_ON;
	Boolean aw_led_n=true;

	Boolean aw_led_s=false;
	
	private FinalCountDownTimer finalCountDownTimer=null;

    private NotificationManager m_NotificationManager;
    private int notificationId =4001;
    Context ctx = this;
    
    /*static {
         System.loadLibrary("gpio_jni");
         nativeInit();
    }*/
    
    private static native void nativeInit();
    private static native int nativeWriteGpio(String path, String value);
    private static native int nativeReadGpio(String path);

    
	public static int setNormalLedOn(boolean on){
	    String dataPath = NORMAL_LED_PATH + mDataName;
	    return nativeWriteGpio(dataPath, on?"1":"0");
	}

	public static int setStandbyLedOn(boolean on){
	    String dataPath = STANDBY_LED_PATH + mDataName;
	    return nativeWriteGpio(dataPath, on?"1":"0");
	}
	
	//MediaPlayer mp = new MediaPlayer(); 

	String[] vm_property = { "java.vm.name", "java.vm.specification.vendor",
			"java.vm.vendor", "java.vm.specification.name",

			"java.specification.name", "java.specification.vendor",

			"java.vendor", "ro.yunos.version",

			"lemur.vm.version" };

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		ClassLoader classLoader = getClassLoader();        
		if (classLoader != null){
		Log.i(TAG, "[onCreate] classLoader : " + classLoader.toString());           
				while (classLoader.getParent()!=null){
	            classLoader = classLoader.getParent();
	            Log.i(TAG,"[onCreate] classLoader : " + classLoader.toString());
	        }
	    }

		
		setContentView(R.layout.activity_main);

		bt = (Button) findViewById(R.id.button_start);
		tx = (TextView) findViewById(R.id.textView_judge);
		bt_datachmod = (Button) findViewById(R.id.button_chmod);
		button_adb = (Button) findViewById(R.id.button_adb);
		button_sevenup = (Button) findViewById(R.id.button_sevenup);
		
		editText_mac = (TextView) findViewById(R.id.editText_mac);
		button_mac = (Button) findViewById(R.id.button_mac);
		

		bt_hisiadbstart =  (Button) findViewById(R.id.button_hisiadbstart);
		bt_hisiadbstop =  (Button) findViewById(R.id.button_hisiadbstop);
		button_hisiled =  (Button) findViewById(R.id.button_hisiled);
		button_hisi_wipe =  (Button) findViewById(R.id.button_hisi_wipe);
		button_himeidi_dev =  (Button) findViewById(R.id.button_himeidi_dev);
		
		button_show_notify = (Button) findViewById(R.id.button_show_notify);
		button_notify_one = (Button) findViewById(R.id.button_notify_one);
		button_clearall  = (Button) findViewById(R.id.button_clearall);
		
		btn_vendorid = (Button) findViewById(R.id.btn_vendorid);
		button_sdcard = (Button) findViewById(R.id.button_sdcard);
		button_routemac = (Button) findViewById(R.id.button_routemac);
		button_media_info = (Button) findViewById(R.id.button_media_info);
		button_bkup = (Button) findViewById(R.id.button_bkup);
		button_dropbox = (Button) findViewById(R.id.button_dropbox);
		button_aw_led_normal = (Button) findViewById(R.id.button_aw_led_normal);
		button_aw_led_standby = (Button) findViewById(R.id.button_aw_led_standby);
		button_rk_led = (Button) findViewById(R.id.button_rk_led);
		button_ill = (Button) findViewById(R.id.button_ill);
		
		bt.setOnClickListener(this);
		bt_datachmod.setOnClickListener(this);
		button_adb.setOnClickListener(this);
		button_bkup.setOnClickListener(this);
		
		button_mac.setOnClickListener(this);
		button_sevenup.setOnClickListener(this);
		
		bt_hisiadbstart.setOnClickListener(this);
		bt_hisiadbstop.setOnClickListener(this);
		button_hisiled.setOnClickListener(this);
		button_hisi_wipe.setOnClickListener(this);
		button_himeidi_dev.setOnClickListener(this);
		button_show_notify.setOnClickListener(this);
		button_notify_one.setOnClickListener(this);
		button_clearall.setOnClickListener(this);
		btn_vendorid.setOnClickListener(this);
		button_sdcard.setOnClickListener(this);
		button_routemac.setOnClickListener(this);
		button_media_info.setOnClickListener(this);
		button_dropbox.setOnClickListener(this);
		button_aw_led_normal.setOnClickListener(this);
		button_aw_led_standby.setOnClickListener(this);
		button_rk_led.setOnClickListener(this);
		button_ill.setOnClickListener(this);
		
		getSystemInfo();
		

	    m_NotificationManager = ((NotificationManager)getSystemService("notification"));
	}

	@Override
	public void onClick(View arg0) {
		if (arg0.getId() == R.id.button_start) {
			startJudge();
		} else if (arg0.getId() == R.id.button_chmod) {

			try {
				execSuCommand("busybox chmod 777 -R /data");
				// String[]{"sh","-c","getprop|grep ip"});
				String resultstr = execCommandArray(new String[] { "sh", "-c",
						"ls -al |grep data" });
				Message m = mhadler.obtainMessage(THREAD, resultstr);
				m.sendToTarget();
			} catch (IOException e) {
				e.printStackTrace();
			}
		} else if (arg0.getId() == R.id.button_adb) {

			try {

				execSuArrayCommand(new String[] {
						"setprop service.adb.tcp.port 5555", "stop adbd",
						"start adbd" });

				/*
				 * String resultstr = execCommandArray(new
				 * String[]{"sh","-c","setprop service.adb.tcp.port 5555"});
				 * System.out.println(resultstr); resultstr =
				 * execCommandArray(new String[]{"sh","-c","stop adbd"});
				 * System.out.println(resultstr); resultstr =
				 * execCommandArray(new String[]{"sh","-c","start adbd"});
				 * System.out.println(resultstr);
				 */
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		} else if (arg0.getId() == R.id.button_mac) {
			try {
				String str = "busybox ifconfig eth0 hw ether "
						+ editText_mac.getText();

				/*execCommand("busybox ifconfig eth0 down");
				execCommand(str);
				execCommand("busybox ifconfig eth0 up");*/
				
				execSuArrayCommand(new String[] {
						"busybox ifconfig eth0 down", str,
						"busybox ifconfig eth0 up" });
				

				/*execSuArrayCommand(new String[] {
						"busybox ifconfig eth0 down"});*/
				
				//execCommandArray(new String[] {"sh","-c","su busybox ifconfig eth0 down"});
				
				/*execCommand("su - root");
				execCommand("busybox ifconfig eth0 down");*/
				
				/*
				 * String resultstr = execCommandArray(new
				 * String[]{"sh","-c","setprop service.adb.tcp.port 5555"});
				 * System.out.println(resultstr); resultstr =
				 * execCommandArray(new String[]{"sh","-c","stop adbd"});
				 * System.out.println(resultstr); resultstr =
				 * execCommandArray(new String[]{"sh","-c","start adbd"});
				 * System.out.println(resultstr);
				 */
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}else if(arg0.getId() == R.id.button_sevenup){
			 //Uri uri = Uri.parse("http://www.upseven.net:8082/wordpress");  
			 //Intent it = new Intent(Intent.ACTION_VIEW, uri);  
			 //startActivity(it);
			 Log.d("jhy",Environment.getExternalStorageDirectory().toString());
			 Log.d("jhy",Environment.getExternalStorageDirectory().getPath());
			 Log.d("jhy",Environment.getExternalStorageDirectory().getAbsolutePath());
			 
			 File file = new File(Environment.getExternalStorageDirectory(), "/jhy.txt");

				FileOutputStream out;
				try {
					out = new FileOutputStream(file);
					out.write("111".getBytes());
					out.flush();
					out.close();
				} catch (FileNotFoundException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				
				ClassLoader classLoader = getClassLoader();
				Log.v("jhy", "classLoader = " + classLoader.toString());
				String ldpath = System.getProperty("java.library.path");
				Log.v("jhy", "ldpath = " + ldpath);

			 
		}else if(arg0.getId() == R.id.button_hisiadbstart){
			new adb_start_Thread().start();
		}else if(arg0.getId() == R.id.button_hisiadbstop){
			new adb_stop_Thread().start();
		}else if(arg0.getId() == R.id.button_hisiled){

			new led_Thread().start();
		}else if(arg0.getId() == R.id.button_hisi_wipe){
			new wipe_Thread().start();
		}else if(arg0.getId() == R.id.button_himeidi_dev){
			new dev_Thread().start();
		}else if(arg0.getId() == R.id.button_show_notify){
			showNotification();
		}else if(arg0.getId() == R.id.button_notify_one){
			addNewNotification();
		}else if(arg0.getId() == R.id.button_clearall){
			clearAllNotification();
		}else if(arg0.getId() == R.id.btn_vendorid){
			String vendorid = "";
			Class localClass;
			try {
				localClass = Class.forName("com.yunos.settings.SystemInfoApiFw");
			      Object localObject2 = localClass.getMethod("getInstance",new Class[] { Context.class }).invoke(null, new Object[] { this });
			      Object localObject3 = localClass.getMethod("getVendorID").invoke(localObject2, new Object[0]);
			      if (localObject3 != null)
			      {
			        String str = localObject3.toString();
			        vendorid = str;
			      }
			} catch (ClassNotFoundException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (IllegalArgumentException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (IllegalAccessException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (InvocationTargetException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (NoSuchMethodException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			
			String uuid = getUUID();
			int s =getSecureBootStatus(ctx);
			

			Toast.makeText(getApplicationContext(), vendorid+","+uuid+","+s,
					Toast.LENGTH_LONG).show();
			
		      Log.i("vendorid:", vendorid);
		}else if(arg0.getId() == R.id.button_sdcard){
			// 得到文件系统的信息：存储块大小，总的存储块数量，可用存储块数量
			// 获取sd卡空间
			// 存储设备会被分为若干个区块
			// 每个区块的大小 * 区块总数 = 存储设备的总大小
			// 每个区块的大小 * 可用区块的数量 = 存储设备可用大小
			File path = Environment.getExternalStorageDirectory();
			StatFs stat = new StatFs(path.getPath());
			long blockSize;
			long totalBlocks;
			long availableBlocks;
			// 由于API18（Android4.3）以后getBlockSize过时并且改为了getBlockSizeLong
			// 因此这里需要根据版本号来使用那一套API


			blockSize = stat.getBlockSize();
			totalBlocks = stat.getBlockCount();
			availableBlocks = stat.getAvailableBlocks();

			// 利用formatSize函数把字节转换为用户等看懂的大小数值单位

			String totalText = formatSize(blockSize * totalBlocks);
			String availableText = formatSize(blockSize * availableBlocks);
			tx.setText("SDCard总大小:\n" + totalText + "SDCard可用空间大小:\n" + availableText);
		}else if(arg0.getId() == R.id.button_routemac){

			new Thread(new getRouteWifiMacJob(this)).start();
		}else if(arg0.getId() == R.id.button_media_info){
			Intent i = new Intent(this,VideoSurface.class);
			startActivity(i);
			
		}else if(arg0.getId() == R.id.button_bkup){
            // 获得ImageView当前显示的图片  
			ImageView imageView = (ImageView) findViewById(R.id.imageView1);
			Bitmap bitmap1 = ((BitmapDrawable) imageView.getBackground()).getBitmap();  
			
			Matrix matrix = new Matrix();
			Bitmap bitmap2 = Bitmap.createBitmap(bitmap1, 0, 0, bitmap1.getWidth(),  
			bitmap1.getHeight(), matrix, true);  
			//如果图片还没有回收，强制回收  
			if (!bitmap1.isRecycled()) {  
			bitmap1.recycle();  
			}  
			//根据原始位图和Matrix创建新的图片  
			imageView.setImageBitmap(bitmap2);  
		}else if(arg0.getId() == R.id.button_dropbox){
			
			DropBoxManager dm = (DropBoxManager)getSystemService(Context.DROPBOX_SERVICE);
			DropBoxManager.Entry e0 = dm.getNextEntry("SYSTEM_RECOVERY_LOG", 0);
			if(e0 == null){
				Log.d("==jhy==", "e0 null");
				return;
			}
			Log.d("==jhy==", "e0:"+e0.getTimeMillis());

			DropBoxManager.Entry e1 = dm.getNextEntry("SYSTEM_RECOVERY_LOG", e0.getTimeMillis());
			while (e1!=null) {
				Log.d("==jhy==", "e1:"+e1.getTimeMillis());
				e0.close();
				e0 = e1;
				e1 = dm.getNextEntry("SYSTEM_RECOVERY_LOG", e0.getTimeMillis());
				
			}
			
			
			
			String txt = e0.getText(128*1024);
			e0.close();
			
	        
		}else if(arg0.getId() == R.id.button_aw_led_normal){
			/*if(aw_led_n.equals(true)){

				aw_led_n = false;
			}else{

				aw_led_n = true;
			}
			setNormalLedOn(aw_led_n);*/
			
			startDowncount();
		}else if(arg0.getId() == R.id.button_aw_led_standby){
			if(aw_led_s.equals(true)){

				aw_led_s = false;
			}else{

				aw_led_s = true;
			}

			setStandbyLedOn(aw_led_s);
			
		}else if(arg0.getId() == R.id.button_rk_led){
			try {
				FileOutputStream out=new FileOutputStream(ledFile);
				if(rk_status.equals(RK_LED_ON)){

					rk_status = RK_LED_OFF;
				}else{

					rk_status = RK_LED_ON;
				}
				out.write(rk_status.getBytes());
				out.flush();
				out.close();
			} catch (FileNotFoundException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}else if(arg0.getId() == R.id.button_ill){
			File f = null;
			f.getAbsoluteFile();
		}
		
		

	}
	
	public String testAudio(){
		AudioManager audiomanage = (AudioManager)ctx.getSystemService(Context.AUDIO_SERVICE);
		audiomanage.getStreamMaxVolume(AudioManager.STREAM_MUSIC);
		return null;
	}
	
	public static String getUUID() {
        try {
            Class<?> cloudUUID = Class
                    .forName("com.yunos.baseservice.clouduuid.CloudUUID");
            Method m = cloudUUID.getMethod("getCloudUUID");
            String result = (String) m.invoke(null);
            //LogUtils.logDebug("result:" + result);
            if (result.equals("false")) {
                return "";
            }
            return result;
        } catch (Exception e) {
            //LogUtils.logError("getUUID is error", SystemInfo.class, e);
        }
        return "";
    }   
	
	public static int getSecureBootStatus(Context context) {

        int secure_boot = -1;
        try {
            Class<?> clazz = Class
                    .forName("com.yunos.settings.SettingApiManager");
            Method method = clazz.getMethod("getInfoCollectionUtils",
                    Context.class);
            Object infoCollectionObj = method.invoke(null, context);
            Method getVendorMethod = infoCollectionObj.getClass().getMethod(
                    "getSecBootStatus");
            Object id = getVendorMethod.invoke(infoCollectionObj);
            if (id != null) {
                secure_boot = ((Integer) id).intValue();
            }    
        } catch (Exception e) { 
            //LogUtils.logError("getSecBootStatus error", SystemInfo.class, e);
        }    
        //LogUtils.logDebug("secure_boot:" + secure_boot,SystemInfo.class);
        return secure_boot;

    }
	
	
	//封装Formatter.formatFileSize方法，具体可以参考安卓的API
	private String formatSize(long size)
	{
		return Formatter.formatFileSize(this, size);
	}

	Handler mhadler = new Handler() {
		public void handleMessage(Message msg) {
			switch (msg.what) {
			case THREAD:
				tx.setText((String) msg.obj);
				break;
			}
			super.handleMessage(msg);
		}
	};
	
	  private static boolean isConnectedToRouter(Context paramContext)
	  {
	    ConnectivityManager localConnectivityManager = (ConnectivityManager)paramContext.getSystemService("connectivity");
	    if (localConnectivityManager != null)
	    {
	      NetworkInfo localNetworkInfo = localConnectivityManager.getNetworkInfo(1);
	      if ((localNetworkInfo != null) && (localNetworkInfo.isConnected())) {
	        return true;
	      }
	    }
	    return false;
	  }

	  private static String mRouterWifiMac = "";
	  
	  public static String getRouterWifiMac(Context paramContext)
	  {
		  mRouterWifiMac = "none";
	    if (paramContext != null)
	    {
	      if (isConnectedToRouter(paramContext)) {
		      WifiManager localWifiManager = (WifiManager)paramContext.getSystemService("wifi");
		      if (localWifiManager != null)
		      {
		        WifiInfo localWifiInfo = localWifiManager.getConnectionInfo();
		        if (localWifiInfo != null)
		        {
		          mRouterWifiMac = localWifiInfo.getBSSID();
		          if (TextUtils.isEmpty(mRouterWifiMac)) {
		            mRouterWifiMac = "NoValue";
		          }
		        }
		      }
	      }else{

			  mRouterWifiMac = "no wifi ap!";
	      }
	    }
	    return mRouterWifiMac;
	  }
	
	class getRouteWifiMacJob implements Runnable {
		Context paramContext;
		
		public getRouteWifiMacJob(Context paramContext) {
			super();
			this.paramContext = paramContext;
		}

		@Override
		public void run() {
			
			while(true){
				String s = getRouterWifiMac(paramContext);
				Log.d("selfdo",s);
				Message m = mhadler.obtainMessage(THREAD, s);
				m.sendToTarget();
				try {
					Thread.sleep(5000);
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}

		}

	}

	class cleanjob implements Runnable {

		@Override
		public void run() {
			try {
				// execCommand("getprop");
				// execCommand("");
				/*
				 * proc =
				 * runtime.exec("chmod 777 -R /mnt/sdcard/DCIM/.thumbnails");
				 * proc.waitFor();
				 */
				// proc = runtime.exec("su -c mkdir /mnt/sdcard/DCIM/ddd");
				// proc = runtime.exec("su -c rm -rf /mnt/sdcard/DCIM/dd");

				execCommandArray(new String[] { "sh", "-c", "getprop|grep ip" });

				execSuCommand("rm -rf /mnt/sdcard/DCIM/.thumbnails");// 删除缩略图区域
				String resultstr = execCommand("ls -al /mnt/sdcard/DCIM/.thumbnails");
				Message m = mhadler.obtainMessage(THREAD, resultstr);
				m.sendToTarget();

				// execCommand("ls");
			} catch (IOException e) {
				e.printStackTrace();
			}

			/*
			 * Properties p = System.getProperties();
			 * 
			 * StringBuilder strb = new StringBuilder(tx.getText()); for (String
			 * str : vm_property) { System.err.println(p.get(str));
			 * 
			 * strb.append(str+":"+p.get(str)+"\n"); }
			 * 
			 * tx.setText(strb.toString());
			 */
		}

	}

	private void startJudge() {
		// Message m = mhadler.obtainMessage(THREAD, "test");
		// m.sendToTarget();
		/*
		 * String action = "android.intent.action.VIEW"; Intent intent = new
		 * Intent(); intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
		 * intent.setAction(action); intent.setData(Uri.parse(
		 * "appstore://start?module=installed&appType=other&catCode=46&catName=儿童天地"
		 * ));
		 * 
		 * startActivity(intent);
		 */

		new Thread(new cleanjob()).start();

	}

	public static String execSuArrayCommand(String[] cmd) throws IOException {

		System.err.println("su执行开始");
		Process process = Runtime.getRuntime().exec("su");
		DataOutputStream os = new DataOutputStream(process.getOutputStream());
		for (String str : cmd) {

			os.writeBytes(str + "\n");
			os.flush();
		}
		os.writeBytes("exit\n");
		os.flush();

		BufferedReader reader = new BufferedReader(new InputStreamReader(
				process.getInputStream()));
		int read;
		char[] buffer = new char[4096];
		StringBuffer output = new StringBuffer();
		while ((read = reader.read(buffer)) > 0) {
			output.append(buffer, 0, read);
		}
		reader.close();
		os.close();

		System.err.println("su执行结束");
		return output.toString();
	}

	public static String execSuCommand(String cmd) throws IOException {

		System.err.println("su执行开始");
		Process process = Runtime.getRuntime().exec("su");
		DataOutputStream os = new DataOutputStream(process.getOutputStream());
		os.writeBytes(cmd + "\n");
		os.flush();
		os.writeBytes("exit\n");
		os.flush();

		BufferedReader reader = new BufferedReader(new InputStreamReader(
				process.getInputStream()));
		int read;
		char[] buffer = new char[4096];
		StringBuffer output = new StringBuffer();
		while ((read = reader.read(buffer)) > 0) {
			output.append(buffer, 0, read);
		}
		reader.close();
		os.close();

		System.err.println("su执行结束");
		return output.toString();
	}

	public String execCommand(String command) throws IOException {
		Runtime runtime = Runtime.getRuntime();
		Process proc = runtime.exec(command);

		System.err.println("执行开始");

		try {
			System.err.println("执行开始2");

			if (proc.waitFor() != 0) {

				System.err.println("exit value1 = " + proc.exitValue());
				BufferedReader reader = new BufferedReader(
						new InputStreamReader(proc.getErrorStream()));
				int read;
				char[] buffer = new char[4096];
				StringBuffer output = new StringBuffer();
				while ((read = reader.read(buffer)) > 0) {
					output.append(buffer, 0, read);
				}
				reader.close();

				System.out.println(output.toString());
				return output.toString();

			} else {

				System.err.println("执行完毕");
			}

			BufferedReader reader = new BufferedReader(new InputStreamReader(
					proc.getInputStream()));
			int read;
			char[] buffer = new char[4096];
			StringBuffer output = new StringBuffer();
			while ((read = reader.read(buffer)) > 0) {
				output.append(buffer, 0, read);
			}
			reader.close();

			System.out.println(output.toString());
			return output.toString();

		} catch (InterruptedException e) {

			System.err.println(e);
			StringWriter sw = new StringWriter();
			PrintWriter pw = new PrintWriter(sw);
			e.printStackTrace(pw);
			return sw.toString();
		}

	}

	public String execCommandArray(String[] command) throws IOException {
		Runtime runtime = Runtime.getRuntime();

		// Process proc = runtime.exec(new
		// String[]{"sh","-c","getprop|grep ip"});
		Process proc = runtime.exec(command);

		System.err.println("execCommandArray开始");
		try {

			if (proc.waitFor() != 0) {

				System.err.println("exit value = " + proc.exitValue());

				if (proc.exitValue() == 1) {

					System.err.println("execCommandArray执行返回无结果或者查询为空");
				}

				BufferedReader reader = new BufferedReader(
						new InputStreamReader(proc.getErrorStream()));
				int read;
				char[] buffer = new char[4096];
				StringBuffer output = new StringBuffer();
				while ((read = reader.read(buffer)) > 0) {
					output.append(buffer, 0, read);
				}
				reader.close();

				System.out.println(output.toString());
				return output.toString();

			} else {
				System.err.println("execCommandArray结束");

			}

			BufferedReader reader = new BufferedReader(new InputStreamReader(
					proc.getInputStream()));
			int read;
			char[] buffer = new char[4096];
			StringBuffer output = new StringBuffer();
			while ((read = reader.read(buffer)) > 0) {
				output.append(buffer, 0, read);
			}
			reader.close();

			System.out.println(output.toString());
			return output.toString();

		} catch (InterruptedException e) {

			System.err.println(e);
			StringWriter sw = new StringWriter();
			PrintWriter pw = new PrintWriter(sw);
			e.printStackTrace(pw);
			return sw.toString();

		}

	}

	public void getSystemInfo() {
		System.err.println(Environment.getExternalStorageDirectory()
				.getAbsolutePath());
		System.err.println((Environment.getExternalStorageDirectory()
				.getTotalSpace() / 1024 / 1024) / 1024.0);
		System.err.println((Environment.getExternalStorageDirectory()
				.getUsableSpace() / 1024 / 1024) / 1024.0);
		System.err.println((Environment.getExternalStorageDirectory()
				.getFreeSpace() / 1024 / 1024) / 1024.0);

		StringBuilder sb = new StringBuilder();
		String sdpath = Environment.getExternalStorageDirectory()
				.getAbsolutePath();
		String totalspace = String
				.valueOf((Environment.getExternalStorageDirectory()
						.getTotalSpace() / 1024 / 1024) / 1024.0);
		String freespace = String
				.valueOf((Environment.getExternalStorageDirectory()
						.getUsableSpace() / 1024 / 1024) / 1024.0);
		sb.append("当前的SD路径:" + sdpath + ",");
		sb.append("总的大小" + totalspace + "G,");
		sb.append("可用大小" + freespace + "G\n");

		try {
			String resultstr = execCommandArray(new String[] { "sh", "-c",
					"getprop|grep dhcp.wlan0.ipaddress" });
			sb.append(resultstr);

		} catch (IOException e) {
			e.printStackTrace();
		}

		try {
			String resultstr = execCommandArray(new String[] { "sh", "-c",
					"getprop|grep dhcp.eth0.ipaddress" });
			sb.append(resultstr);

		} catch (IOException e) {
			e.printStackTrace();
		}
		
		((TextView) findViewById(R.id.textView_ShowSystemInfo)).setText(sb
				.toString());
	}
	
	///////////////////////////////////海思adb 打开关闭///////////////////////////////////////
    class adb_start_Thread extends Thread
    {
      adb_start_Thread()
      {
      }

      public void run()
      {
        super.run();
        SocketClient localSocketClient = new SocketClient();
        localSocketClient.writeMess("system start adbd ");
        localSocketClient.readNetResponseSync();
      }
    }

    class adb_stop_Thread extends Thread
    {
      adb_stop_Thread()
      {
      }

      public void run()
      {
        super.run();
        SocketClient localSocketClient = new SocketClient();
        localSocketClient.writeMess("system stop adbd ");
        localSocketClient.readNetResponseSync();
      }
    }
    
    class wipe_Thread extends Thread
    {
    	wipe_Thread()
      {
      }

      public void run()
      {
        super.run();
        SocketClient localSocketClient = new SocketClient();
        localSocketClient.writeMess("system write_raw misc wipe_data ");
        localSocketClient.readNetResponseSync();
      }
    }
    
    class led_Thread extends Thread
    {
    	led_Thread()
      {
      }

      public void run()
      {
        super.run();
        SocketClient localSocketClient = new SocketClient();
        localSocketClient.writeMess("system gpio-led");
        localSocketClient.readNetResponseSync();
      }
    }
    
    class dev_Thread extends Thread
    {
    	dev_Thread()
      {
      }

      public void run()
      {
        super.run();
        SocketClient localSocketClient = new SocketClient();
        localSocketClient.writeMess("system am start  com.yunos.tv.devicemanager/.DeveloperMode");
        localSocketClient.readNetResponseSync();
      }
    }
    
    
    
    @Override
	public void onWindowFocusChanged(boolean hasFocus) {
		// TODO Auto-generated method stub
    	Log.d("===start===", "hasFocus : "+hasFocus);
		super.onWindowFocusChanged(hasFocus);
	}
	private void showNotification()
    {
		try {
			/*Object service = getSystemService("statusbar");
			Class<?> statusBarManager = Class
					.forName("android.app.StatusBarManager");
			int versionNum = android.os.Build.VERSION.SDK_INT;
			Method expand = null;
			if (versionNum < 17)
				expand = statusBarManager.getMethod("expand");
			else
				expand = statusBarManager.getMethod("expandNotificationsPanel");
	
			if (expand != null) {
				expand.setAccessible(true);
				expand.invoke(service);
			}*/
	
			Object service = getSystemService("statusbar");
			/*Class<?> statusBarManager = Class
					.forName("com.android.server.StatusBarManagerService");*/
			Class<?> statusBarManager = Class
					.forName("android.app.StatusBarManager");
			
			int versionNum = android.os.Build.VERSION.SDK_INT;
			Method expand = null;
			if (versionNum < 17)
				expand = statusBarManager.getMethod("expand");
			else
				expand = statusBarManager.getMethod("expandNotificationsPanel");
	
			if (expand != null) {
				expand.setAccessible(true);
				expand.invoke(service);
			}
			
		} catch (Exception e) {
			e.printStackTrace();
			Toast.makeText(getApplicationContext(), e.getMessage(),
					Toast.LENGTH_LONG).show();
			System.err.println("执行错误");
		}
    }
    
    private void clearAllNotification()
    {
      m_NotificationManager.cancelAll();
    }
    
    private void addNewNotification()
    {
      Notification localNotification = new Notification();
      PendingIntent localPendingIntent = PendingIntent.getActivity(this, 0, new Intent(this, MainActivity.class), 0);


        localNotification.tickerText = ("Notification " + notificationId);
        localNotification.defaults = (0x1 | localNotification.defaults);
        localNotification.flags = (0x10 | localNotification.flags);
        localNotification.icon = R.drawable.ic_launcher; 
        
        localNotification.setLatestEventInfo(this, "Notification " + notificationId, "Notification detail", localPendingIntent);
        m_NotificationManager.notify(notificationId, localNotification);
        notificationId = (1 + notificationId);
        Log.e("TEST", "send notify");
        //sendBroadcast();
    }
    
    private void sendBroadcast()
    {
      Intent localIntent = new Intent("system.ui.notification.count");
      localIntent.putExtra("nitify.count", this.notificationId);
      sendBroadcast(localIntent);
    }
    
    public void startDowncount() {
    	Log.d(TAG, "startDowncount task=" + finalCountDownTimer);
        //if(null == finalCountDownTimer) {
            finalCountDownTimer = new FinalCountDownTimer(15 * 1000 , 100);
            finalCountDownTimer.start();
        //}
    }
    
    private final class FinalCountDownTimer extends android.os.CountDownTimer{

		public FinalCountDownTimer(long millisInFuture, long countDownInterval) {
			super(millisInFuture, countDownInterval);
			// TODO Auto-generated constructor stub
		}

		@Override
		public void onFinish() {
			// TODO Auto-generated method stub

			Log.d(this.getClass().getName(), "done!");
		}

		@Override
		public void onTick(long arg0) {
			// TODO Auto-generated method stub
			Log.d(this.getClass().getName(), "left time:"+arg0);
			
		}
    	
    }
}
