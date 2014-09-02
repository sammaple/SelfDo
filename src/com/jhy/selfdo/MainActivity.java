package com.jhy.selfdo;

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.io.StringWriter;

import android.app.Activity;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.Message;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;

public class MainActivity extends Activity implements OnClickListener {

	protected static final int THREAD = 0;
	Button bt;
	Button bt_datachmod,button_adb;
	TextView tx;

	String[] vm_property = { "java.vm.name", "java.vm.specification.vendor",
			"java.vm.vendor", "java.vm.specification.name",

			"java.specification.name", "java.specification.vendor",

			"java.vendor", "ro.yunos.version",

			"lemur.vm.version" };

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		bt = (Button) findViewById(R.id.button_start);
		tx = (TextView) findViewById(R.id.textView_judge);
		bt_datachmod = (Button) findViewById(R.id.button_chmod);
		button_adb= (Button) findViewById(R.id.button_adb);
		bt.setOnClickListener(this);
		bt_datachmod.setOnClickListener(this);
		button_adb.setOnClickListener(this);
		
		getSystemInfo();
	}

	@Override
	public void onClick(View arg0) {
		if (arg0.getId() == R.id.button_start) {
			startJudge();
		} else if (arg0.getId() == R.id.button_chmod) {

			try {
				execSuCommand("busybox chmod 777 -R /data");
				// String[]{"sh","-c","getprop|grep ip"});
				String resultstr = execCommandArray(new String[]{"sh","-c","ls -al |grep data"});
				Message m = mhadler.obtainMessage(THREAD, resultstr);
				m.sendToTarget();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}else if(arg0.getId() == R.id.button_adb){

			try {
				
				execSuArrayCommand(new String[]{"setprop service.adb.tcp.port 5555","stop adbd","start adbd"});
				
				/*String resultstr = execCommandArray(new String[]{"sh","-c","setprop service.adb.tcp.port 5555"});
				System.out.println(resultstr);
				resultstr = execCommandArray(new String[]{"sh","-c","stop adbd"});
				System.out.println(resultstr);
				resultstr = execCommandArray(new String[]{"sh","-c","start adbd"});
				System.out.println(resultstr);*/
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}

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
		/*String action = "android.intent.action.VIEW";
		Intent intent = new Intent();
        intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        intent.setAction(action);
        intent.setData(Uri.parse("appstore://start?module=installed&appType=other&catCode=46&catName=儿童天地"));
        
        startActivity(intent);*/

	    new Thread(new cleanjob()).start();

	}

	public static String execSuArrayCommand(String[] cmd) throws IOException {

		System.err.println("su执行开始");
		Process process = Runtime.getRuntime().exec("su");
		DataOutputStream os = new DataOutputStream(process.getOutputStream());
		for(String str:cmd){

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
	
	public void getSystemInfo(){
		System.err.println(Environment.getExternalStorageDirectory().getAbsolutePath());
		System.err.println((Environment.getExternalStorageDirectory().getTotalSpace()/1024/1024)/1024.0);
		System.err.println((Environment.getExternalStorageDirectory().getUsableSpace()/1024/1024)/1024.0);
		System.err.println((Environment.getExternalStorageDirectory().getFreeSpace()/1024/1024)/1024.0);
		
		StringBuilder sb = new StringBuilder();
		String sdpath = Environment.getExternalStorageDirectory().getAbsolutePath();
		String totalspace =String.valueOf((Environment.getExternalStorageDirectory().getTotalSpace()/1024/1024)/1024.0) ;
		String freespace = String.valueOf((Environment.getExternalStorageDirectory().getUsableSpace()/1024/1024)/1024.0);
		sb.append("当前的SD路径:"+sdpath+",");
		sb.append("总的大小"+totalspace+"G,");
		sb.append("可用大小"+freespace+"G\n");
		

		try {
			String resultstr = execCommandArray(new String[]{"sh","-c","getprop|grep dhcp.wlan0.ipaddress"});
			sb.append(resultstr);
			
		} catch (IOException e) {
			e.printStackTrace();
		}
		
		((TextView) findViewById(R.id.textView_ShowSystemInfo)).setText(sb.toString());
	}

}
