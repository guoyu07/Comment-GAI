/*!
 @header    GAI.h
 @abstract  Google Analytics iOS SDK Header
 @version   2.0
 @copyright Copyright 2011 Google Inc. All rights reserved.
 */

#import <Foundation/Foundation.h>
#import "GAITracker.h"
#import "GAITrackedViewController.h"

/*! 产品名称 */
extern NSString *const kGAIProduct;

/*! 版本信息 */
extern NSString *const kGAIVersion;

/*!
 当错误发生的时候, 可以使用此 error domain 来跟踪错误发生的情况
 */
extern NSString *const kGAIErrorDomain;

/*! GA错误代码*/
typedef enum {
  // 没有错误发生
  kGAINoError = 0,

  // 数据库相关的错误
  kGAIDatabaseError,

  // 网络错误
  kGAINetworkError,
} GAIErrorCode;

/*!
 Google Analytics iOS top-level class. Provides facilities to create trackers
 and set behaviorial flags.a
 */
@interface GAI : NSObject

/*!
 获取默认的 Tracker , 此属性初始化的时候为 nil, 当多次调用 trackerWithTrackingId 方法的时候, 
 此属性为第一次设置的 TrackingId 的实例化对象. 
 此属性可以被重写. 
 GAITrackedViewController 在默认情况下, 使用此属性来作为 tracker 实例化对象
 */
@property(nonatomic, assign) id<GAITracker> defaultTracker;

/*!
 是否进入 DEBUG 模式, 此属性默认情况下 为 'NO' , 如果手动设置为 'YES' 的话, 会 NSLog 一些调试信息, 
 大部分为 GA 内部调用及发送请求到服务器的信息
*/
@property(nonatomic, assign) BOOL debug;

/*!
 此变量用来控制 GAI 收集信息, 默认为 'NO', 设置为 'YES' 的时候, GAI 将停止收集信息, 
 所有的 GAI 内部指令都会变为无效指令. 
 这个标记一直存活在 sdk 里面, 你可以利用此标记来控制 GA 的信息收集, 如: 在某种私密情况下设
 置为 'YES' 停止 GAI 收集信息, 然后在离开此私密情况后设置为 'NO' 继续收集接下来的信息. 
 */
@property(nonatomic, assign) BOOL optOut;

/*!
 分发信息的间距, 以 '秒' 为单位. 会出现以下三种情况: 
1. 小于 0 , 必须手动调用方法 dispatch 向服务器提交数据; 
2. 等于 0 , 统计信息会实时发送给服务器; 
3. 大于 0 , 会在每隔 此值 的时间向服务器发送请求. 
 默认的时间为 120 秒
 */
@property(nonatomic, assign) NSTimeInterval dispatchInterval;

/*!
 是否跟踪未捕获的 '异常'
 */
@property(nonatomic, assign) BOOL trackUncaughtExceptions;

/*! 类方法, 单例模式的实现, 使用此方法获得默认的 traker 实例 (defaultTracker)*/
+ (GAI *)sharedInstance;

/*!
 创建 trackingId 对应的 GAITracker 实例, 同一个 trackingId 多次调用此方法, 只会创建
 同一个 GAITracker 实例. 
@param trackingId 以 "UA-" 开头的字串, 在 GA 后台创建. 必须有值
@return 返回 trackingId 对应的 GAITracker 实例. 第一次 trackingId 传入的时候会创建新的
GAITracker 实例, 随后的使用同一个 trackingId 传入调用的时候, 会返回此 trackingId 对应的实例
 当有错误发生的时候, 会返回 nil 
 */
- (id<GAITracker>)trackerWithTrackingId:(NSString *)trackingId;

/*! 
 手动调用分发统计信息, 也就是向服务器提交信息, 此方法不会被 dispatchInterval 限制. 
*/
- (void)dispatch;

@end
