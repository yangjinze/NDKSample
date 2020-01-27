package com.yjz.ndk.sample.util;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.google.gson.reflect.TypeToken;
import org.json.JSONObject;

import java.lang.reflect.Type;
import java.util.*;

/**
 * Created by yjz
 */
public class JsonUtils {
  private static Gson gson = new GsonBuilder().setPrettyPrinting().create();
  private static Gson annotationGson = new GsonBuilder().excludeFieldsWithoutExposeAnnotation() // 不导出实体中没有用@Expose注解的属性
      .setPrettyPrinting().create();

  private static final TypeToken<HashMap<String, Object>> mapTokenObject = new TypeToken<HashMap<String, Object>>() {
  };

  public static Type typeOfStrList = new TypeToken<List<String>>() {
  }.getType();

  // HashMap TypeToken
  private static final TypeToken<HashMap<String, String>> mapToken = new TypeToken<HashMap<String, String>>() {
  };
  private static final TypeToken<LinkedHashMap<String, String>> linkedmapToken = new TypeToken<LinkedHashMap<String, String>>() {
  };
  public static <T> T fromJson(String json, Class<T> classOfT) {
    try {
      T t = gson.fromJson(json, classOfT);
      return t;
    } catch (Exception e) {
      e.printStackTrace();
    }
    return null;
  }

  public static <T> T fromJson(String json, Type typeOfT) {
    try {
      T t = gson.fromJson(json, typeOfT);
      return t;
    } catch (Exception e) {
      e.printStackTrace();
    }
    return null;
  }

  public static String toJson(Object src) {
    try {
      String json = gson.toJson(src);
      return json;
    } catch (Exception e) {
      e.printStackTrace();
    }
    return null;
  }

  public static String toJson(Object src, Type typeOfT) {
    try {
      String json = gson.toJson(src, typeOfT);
      return json;
    } catch (Exception e) {
      e.printStackTrace();
    }
    return null;
  }

  public static String toJsonByAnnotation(Object src) {
    try {
      String json = annotationGson.toJson(src);
      return json;
    } catch (Exception e) {
      e.printStackTrace();
    }
    return null;
  }

  /**
   * 将Map转换成Json字符串
   * 
   * @param map
   *          要转换的Map
   * @return Json字符串
   */
  public static String map2Json(Map<String, String> map) {
    if (map == null) {
      return null;
    }
    String json = "";
    try{
      json = gson.toJson(map);
    }catch (Exception e){
      e.printStackTrace();
    }
    return json;
  }

  /**
   * 将Json字符串转换为Map
   * 
   * @param json
   *          要转换的Json字符串
   * @return 通过json转换的Map
   */
  public static HashMap<String, String> json2Map(String json) {
    if (json == null) {
      return null;
    }
    HashMap<String, String> map = new HashMap<String,String>();
    try{
      map = gson.fromJson(json, mapToken.getType());
    }catch (Exception e){
      e.printStackTrace();
    }
    return map;
  }

  /**
   * 嵌套的json转map (此时用json2Map会保存gson转换的异常)
   *
   * 比如 {"errormessage":1,"merchant":{"merchantId":"103881310000981"}}
   * 转换后map:{merchant={"merchantId":"103881310000981"}, errormessage=1}
   * @param json
   * @return
   */
  public static HashMap<String, String> nestedJson2Map(String json) {
    if (json == null) {
      return null;
    }
    HashMap<String, String> map = new HashMap<String,String>();
    try{
      JSONObject jsonObject = new JSONObject(json);
      for (Iterator<String> iterator = jsonObject.keys(); iterator.hasNext();) {
        String key = iterator.next();
        map.put(key, jsonObject.getString(key));
      }
    }catch (Exception e){
      e.printStackTrace();
    }
    return map;
  }



  public static LinkedHashMap<String, String> json2LinkedMap(String json) {
    if (json == null) {
      return null;
    }
    LinkedHashMap<String, String> map = new LinkedHashMap<String,String>();
    try {
      map = gson.fromJson(json, linkedmapToken.getType());
    }catch (Exception e){
      e.printStackTrace();
    }
    return map;
  }
  /**
   * 将Json字符串存入Map中
   * 
   * @param json
   *          Json字符串
   * @param map
   *          存放结果的Map
   */
  public static boolean json2Map(String json, HashMap<String, String> map) {
    if (json == null || map == null) {
      return false;
    }
    try{
      HashMap<String, String> tempMap = gson.fromJson(json, mapToken.getType());
      Set<String> keySet = tempMap.keySet();
      for (String key : keySet) {
        map.put(key, tempMap.get(key));
      }
    }catch (Exception e){
      e.printStackTrace();
      return false;
    }
    return true;
  }

  public static HashMap<String, Object> json2ObjectMap(String json) {
    if (json == null) {
      return null;
    }
    HashMap<String, Object> map = new HashMap<String, Object>();
    try {
       map = gson.fromJson(json, mapTokenObject.getType());
    }catch (Exception e){
      e.printStackTrace();
    }
    return map;
  }

  public static String mapObject2Json(HashMap<String, Object> map){
    if (map == null) {
      return null;
    }
    String json = "";
    try{
      json = gson.toJson(map);
    }catch (Exception e){
      e.printStackTrace();
    }
    return json;
  }

}
