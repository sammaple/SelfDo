package com.jhy.selfdo;

/*该文件取自海美迪 的RMVB的破解版本*/

import java.lang.reflect.Constructor;
import java.lang.reflect.Method;

public class DeviceInfo
{
  private static boolean checkMethodInClass(String paramString1, String paramString2)
  {
    int i;
    int j;
    do
      try
      {
        Method[] arrayOfMethod = Class.forName(paramString1).getDeclaredMethods();
        i = arrayOfMethod.length;
        j = 0;
        boolean bool = arrayOfMethod[j].getName().equals(paramString2);
        if (bool)
          return true;
        ++j;
      }
      catch (Exception localException)
      {
        return false;
      }
    while (j < i);
    return false;
  }

  public static void doSystemCmd(String paramString)
  {
    if (!checkMethodInClass("android.os.Build", "doSystemCMD"))
      return;
    setBuildMethod("doSystemCMD", paramString);
  }

  public static String getBuildMethod(String paramString)
  {
    try
    {
      Class localClass = Class.forName("android.os.Build");
      Object localObject = localClass.getConstructor(new Class[0]).newInstance(new Object[0]);
      String str = (String)localClass.getMethod(paramString, new Class[0]).invoke(localObject, new Object[0]);
      return str;
    }
    catch (Exception localException)
    {
    }
    return null;
  }

  public static String getChipsetType()
  {
    if (checkMethodInClass("android.os.Build", "getChipsetType"))
      return getBuildMethod("getChipsetType");
    return getSystemProperty("ro.product.himedia.chipset");
  }

  public static String getSystemProperty(String paramString)
  {
    try
    {
      Method localMethod = Class.forName("android.os.SystemProperties").getMethod("get", new Class[] { String.class, String.class });
      Object[] arrayOfObject = new Object[2];
      arrayOfObject[0] = paramString;
      String str = (String)localMethod.invoke(null, arrayOfObject);
      return str;
    }
    catch (Exception localException)
    {
    }
    return null;
  }

  public static void setBuildMethod(String paramString1, String paramString2)
  {
    try
    {
      Class localClass = Class.forName("android.os.Build");
      Object localObject = localClass.getConstructor(new Class[0]).newInstance(new Object[0]);
      localClass.getMethod(paramString1, new Class[] { String.class }).invoke(localObject, new Object[] { paramString2 });
      return;
    }
    catch (Exception localException)
    {
    }
  }

  public static void setBuildMethod_extern(String paramString1, String paramString2, String paramString3)
  {
    try
    {
      Class localClass = Class.forName("android.os.Build");
      Object localObject = localClass.getConstructor(new Class[0]).newInstance(new Object[0]);
      localClass.getMethod(paramString1, new Class[] { String.class, String.class }).invoke(localObject, new Object[] { paramString2, paramString3 });
      return;
    }
    catch (Exception localException)
    {
    }
  }

  public static void setSystemProperty(String paramString1, String paramString2)
  {
    try
    {
      Class.forName("android.os.SystemProperties").getMethod("set", new Class[] { String.class, String.class }).invoke(null, new Object[] { paramString1, paramString2 });
      return;
    }
    catch (Exception localException)
    {
    }
  }
}
