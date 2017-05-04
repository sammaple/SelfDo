package com.jhy.selfdo;

import java.io.File;
import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

import android.app.Activity;
import android.media.MediaPlayer;
import android.media.MediaPlayer.OnCompletionListener;
import android.media.MediaPlayer.OnErrorListener;
import android.media.MediaPlayer.OnInfoListener;
import android.media.MediaPlayer.OnPreparedListener;
import android.media.MediaPlayer.OnSeekCompleteListener;
import android.media.MediaPlayer.OnVideoSizeChangedListener;
import android.os.Bundle;
import android.os.Environment;
import android.os.Parcel;
import android.os.storage.StorageManager;
import android.util.Log;
import android.view.Display;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.widget.LinearLayout;
import android.widget.Toast;
/**
 * 该实例中使用MediaPlayer完成播放，同时界面使用SurfaceView来实现
 * 
 * 这里我们实现MediaPlayer中很多状态变化时的监听器
 * 
 * 使用Mediaplayer时，也可以使用MediaController类，但是需要实现MediaController.mediaController接口
 * 实现一些控制方法。
 * 
 * 然后，设置controller.setMediaPlayer(),setAnchorView(),setEnabled(),show()就可以了，这里不再实现
 * @author Administrator
 *
 */


public class VideoSurface extends Activity implements OnCompletionListener,OnErrorListener,OnInfoListener,
	OnPreparedListener, OnSeekCompleteListener,OnVideoSizeChangedListener,SurfaceHolder.Callback{
	private Display currDisplay;
	private SurfaceView surfaceView;
	private SurfaceHolder holder;
	private MediaPlayer player;
	private int vWidth,vHeight;
	
	
	//private boolean readyToPlay = false;
	public static final int	YUNOS_SOURCE = 0x1f000000;
	public static final int YUNOS_TS_INFO = 0x1f000001;
	public static final int YUNOS_HTTP_DOWNLOAD_ERROR_INFO = 0x1f000002;
	public static final int YUNOS_MEDIA_INFO = 0x1f000003;
	
	public void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);
		this.setContentView(R.layout.video_surface);
		
		
		surfaceView = (SurfaceView)this.findViewById(R.id.video_surface);
		//给SurfaceView添加CallBack监听
		holder = surfaceView.getHolder();
		holder.addCallback(this);
		//为了可以播放视频或者使用Camera预览，我们需要指定其Buffer类型
		holder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);
		
		//下面开始实例化MediaPlayer对象
		player = new MediaPlayer();
		player.setOnCompletionListener(this);
		player.setOnErrorListener(this);
		player.setOnInfoListener(this);
		player.setOnPreparedListener(this);
		player.setOnSeekCompleteListener(this);
		player.setOnVideoSizeChangedListener(this);
		Log.v("Begin:::", "surfaceDestroyed called");
		
		//然后指定需要播放文件的路径，初始化MediaPlayer
		//String dataPath = Environment.getExternalStorageDirectory().getPath()+"/Test_Movie.m4v";
		//String dataPath = "http://shuyu-transcode-public.cn-hangzhou.oss-pub.aliyun-inc.com/switch-stream/pingfan_265.m3u8";
		String dataPath = "http://selinux.cn-hangzhou.oss-pub.aliyun-inc.com/media/DISCONTINUITY/1_aacyizhi.m3u8";
		
		try {
			player.setDataSource(dataPath);
			Log.v("Next:::", "surfaceDestroyed called");
		} catch (IllegalArgumentException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IllegalStateException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		//然后，我们取得当前Display对象
		currDisplay = this.getWindowManager().getDefaultDisplay();
	}
	
	@Override
	public void surfaceChanged(SurfaceHolder arg0, int arg1, int arg2, int arg3) {
		// 当Surface尺寸等参数改变时触发
		Log.v("Surface Change:::", "surfaceChanged called");
	}
	@Override
	public void surfaceCreated(SurfaceHolder holder) {
		// 当SurfaceView中的Surface被创建的时候被调用
		//在这里我们指定MediaPlayer在当前的Surface中进行播放
		player.setDisplay(holder);
		//在指定了MediaPlayer播放的容器后，我们就可以使用prepare或者prepareAsync来准备播放了
		player.prepareAsync();
		
	}
	@Override
	public void surfaceDestroyed(SurfaceHolder holder) {
		
		Log.v("Surface Destory:::", "surfaceDestroyed called");
	}
	@Override
	public void onVideoSizeChanged(MediaPlayer arg0, int arg1, int arg2) {
		// 当video大小改变时触发
		//这个方法在设置player的source后至少触发一次
		Log.v("Video Size Change", "onVideoSizeChanged called");
		
	}
	@Override
	public void onSeekComplete(MediaPlayer arg0) {
		// seek操作完成时触发
		Log.v("Seek Completion", "onSeekComplete called");
		
	}
	@Override
	public void onPrepared(MediaPlayer player) {
		// 当prepare完成后，该方法触发，在这里我们播放视频
		
		//首先取得video的宽和高
		vWidth = player.getVideoWidth();
		vHeight = player.getVideoHeight();
		Log.v("jhy", "in onPrepared!");
		
		//if(vWidth > currDisplay.getWidth() || vHeight > currDisplay.getHeight())
		{
			//如果video的宽或者高超出了当前屏幕的大小，则要进行缩放
			float wRatio = (float)vWidth/(float)currDisplay.getWidth();
			float hRatio = (float)vHeight/(float)currDisplay.getHeight();
			
			//选择大的一个进行缩放
			float ratio = Math.max(wRatio, hRatio);
			
			vWidth = (int)Math.ceil((float)vWidth/ratio);
			vHeight = (int)Math.ceil((float)vHeight/ratio);
			
			//设置surfaceView的布局参数
			surfaceView.setLayoutParams(new LinearLayout.LayoutParams(vWidth, vHeight));
			
			//然后开始播放视频
			
			player.start();
			Log.v("jhy", "start video!");
			

			Class<?> sm;
			try {
				sm = Class.forName("android.media.MediaPlayer");
				Method media_invoke = sm.getMethod("invoke",new Class<?>[]{Parcel.class,Parcel.class});
				media_invoke.setAccessible(true);
				
				//StorageManager service = (StorageManager) getSystemService(STORAGE_SERVICE);
				//File DATA_PATH = Environment.getDataDirectory();
				
				Parcel request = Parcel.obtain();
		        Parcel reply = Parcel.obtain();
		        try {
		            request.writeInterfaceToken("android.media.IMediaPlayer");//privte para in MediaPlayerr
		            request.writeInt(YUNOS_SOURCE);
		            request.writeString("891FE2C2FB7C9C4968E2AD0E99222C58");//写入UUID
		            //invoke(request, reply);

		            //H3H8 打印出来已经写进去了
		            /*06-13 18:24:06.160 W/awplayer(  115): <DecodeAudioStream:822>: get audio decoder change ch or fs!
		            06-13 18:24:06.160 D/awplayer(  115): <DecodeAudioStream:831>: ==============================pAudioDecoder->pBsInFor->bitrate:95779
		            06-13 18:24:06.160 D/awplayer(  115): <invoke:1356>:  YUNOSINVOKE mYunOSUUID[891FE2C2FB7C9C4968E2AD0E99222C58]
		            06-13 18:24:06.160 D/CdxHlsParser(  115): <HlsParserControl:1920>:  HlsParserControl[891FE2C2FB7C9C4968E2AD0E99222C58]*/
		            		
					Object o = media_invoke.invoke(player,new Object[]{request,reply});
		            //TrackInfo trackInfo[] = reply.createTypedArray(TrackInfo.CREATOR);
		            //return trackInfo;
		        } finally {
		            request.recycle();
		            reply.recycle();
		        }

		        
				//long low_ = ((Long)thread) / (1024*1024);

				//Toast.makeText(this, "low thread "+low_+"MB", 5000).show();
				
			} catch (ClassNotFoundException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (NoSuchMethodException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (IllegalAccessException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (IllegalArgumentException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (InvocationTargetException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			
		}
	}
	@Override
	public boolean onInfo(MediaPlayer player, int whatInfo, int extra) {
		// 当一些特定信息出现或者警告时触发
		Log.v("jhy", "whatInfo:"+whatInfo);
		
		Class<?> sm;
		try {
			sm = Class.forName("android.media.MediaPlayer");
			Method media_invoke = sm.getMethod("invoke",new Class<?>[]{Parcel.class,Parcel.class});
			media_invoke.setAccessible(true);
			
			//StorageManager service = (StorageManager) getSystemService(STORAGE_SERVICE);
			//File DATA_PATH = Environment.getDataDirectory();
			
			Parcel request = Parcel.obtain();
	        Parcel reply = Parcel.obtain();
	        try {
	            request.writeInterfaceToken("android.media.IMediaPlayer");//privte para in MediaPlayerr
	            request.writeInt(YUNOS_TS_INFO);
	            //request.writeString("891FE2C2FB7C9C4968E2AD0E99222C58");//写入UUID
	            //invoke(request, reply);

	            //H3H8 打印出来已经写进去了,打印的INFO
	            /*yunos_ts_info:event=3005&ts_duration=9930000&ts_length=2548152&bitrate=1892000&ts_traceid=891FE2C2FB7C9C4968E2AD0E99222C5814658139500101007&ts_send_time=1465813949461&ts_first_btime=1465813949496&ts_last_btime=1465813950009&ts_num=7&cdn_ip=42.120.158.121*/
	            
				Object o = media_invoke.invoke(player,new Object[]{request,reply});
				String yunos_ts_info = reply.readString();
				Log.v("jhy","yunos_ts_info:"+yunos_ts_info);
	            //TrackInfo trackInfo[] = reply.createTypedArray(TrackInfo.CREATOR);
	            //return trackInfo;
	        } finally {
	            request.recycle();
	            reply.recycle();
	        }

	        
			//long low_ = ((Long)thread) / (1024*1024);

			//Toast.makeText(this, "low thread "+low_+"MB", 5000).show();
			
		} catch (ClassNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (NoSuchMethodException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IllegalAccessException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IllegalArgumentException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (InvocationTargetException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		switch(whatInfo){
		
			case MediaPlayer.MEDIA_INFO_BAD_INTERLEAVING:
				break;
			case MediaPlayer.MEDIA_INFO_METADATA_UPDATE:
				
				break;
			case MediaPlayer.MEDIA_INFO_VIDEO_TRACK_LAGGING:
				
				break;
			case MediaPlayer.MEDIA_INFO_NOT_SEEKABLE:
				
				break;
			case YUNOS_MEDIA_INFO:
	
				Log.v("jhy", "YUNOS_MEDIA_INFO!");
				break;
		}
		return false;
	}
	@Override
	public boolean onError(MediaPlayer player, int whatError, int extra) {
		Log.v("Play Error:::", "onError called");
		switch (whatError) {
		case MediaPlayer.MEDIA_ERROR_SERVER_DIED:
			Log.v("Play Error:::", "MEDIA_ERROR_SERVER_DIED");
			break;
		case MediaPlayer.MEDIA_ERROR_UNKNOWN:
			Log.v("Play Error:::", "MEDIA_ERROR_UNKNOWN");
			break;
		default:
			break;
		}
		return false;
	}
	@Override
	public void onCompletion(MediaPlayer player) {
		// 当MediaPlayer播放完成后触发
		Log.v("Play Over:::", "onComletion called");
		this.finish();
	}
}
