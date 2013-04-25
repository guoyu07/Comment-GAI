/*!
 @header    GAITracker.h
 @abstract  Google Analytics iOS SDK Tracker Header
 @version   2.0
 @copyright Copyright 2011 Google Inc. All rights reserved.
*/

#import <Foundation/Foundation.h>
#import "GAITransaction.h"

/*!
 GA 统计信息操作对象, 可以通过 [GAI sharedInstance] 获取默认的实例化对象, 或者 
 [GAI trackerWithTrackingId:] 获取指定 trackingId 对应的实例化对象. 
 可用来统计 屏幕 screens, 事件 events, 交易 transactions, 时间 timing, 异常 exceptions.
 此接口的实现为线程安全 (thread-safe), 方法调用不会屏蔽主线程, 或者是花费很长时间, 所有的
 网络和本地操作都是后台运行. 
 */
@protocol GAITracker<NSObject>

/*!
 通过 [GAI trackerWithTrackingId:] 传进来的 trackingId , 此属性为只读. 
 */
@property(nonatomic, copy, readonly) NSString *trackingId;

/*!
 应用的名称, 默认情况下是从 CFBundleName 里面拿到的, 需要手动更改的话, 请在调用其他方法前重写此属性. 
 */
@property(nonatomic, copy) NSString *appName;

/*!
 程序的 identifier , 此属性应该被为 iTunes Connect application identifier , 默认情况下
 此值为 nil, 需要手动更改的话, 请在调用其他方法前重写此属性. 
 注意: 这里不是应用的 bundle id (e.g. com.example.appname)
 */
@property(nonatomic, copy) NSString *appId;

/*! 
 当前统计的应用的版本号, 默认使用 CFBundleShortVersionString , 需要手动更改的话, 请
 在调用其他方法前重写此属性. 
 */
@property(nonatomic, copy) NSString *appVersion;

/*! 
 是否匿名, 默认为 'NO' , 如果为 'YES' 的话, 统计信息的 IP 地址最后面的一个数字会被 *　代替.
 */
@property(nonatomic, assign) BOOL anonymize;

/*! 
 是否使用 https 加密传输数据, 默认情况下是 'YES', 设置为 'NO' 的话会使用 http 来传输, 
 需要知道的是, 当数据传输给服务器的时候, 同样的数据 https 使用的带宽要高于 http
 */
@property(nonatomic, assign) BOOL useHttps;

/*! 
 采集信息的抽样率, 默认是 100% , 设置为 90 意味着 90% 的抽样率. 
 TODO : why use sampleRate? 
 */
@property(nonatomic, assign) double sampleRate;

/*! 
 设备唯一值, 类似于 udid, 不同的是重装后会不一样, 如下的一串字符串
	696184c5-0334-42d6-9f96-c789da9db083
TODO: 程序更新后是否会变? 
 */
@property(nonatomic, copy, readonly) NSString *clientId;

/*! 
 当前的 屏幕 (screen) 名称, 在 GA 统计后台查看数据时候某个屏幕停留了多长时间就是这个属性
 起的作用. 
 调用 trackView: 的时候, 会在统计信息发送到服务器之前更新此属性. 
 可以直接设置此属性的值, 在下一次 dispatche 的时候, 就会使用此值作为 screen 名称, 发送到服务器. 
 */
@property(nonatomic, copy) NSString *appScreen;

/*! 
 渠道统计的来源统计 campaign Referrals, 需要应用支持 Custom URL Scheme 来实现
 */
@property(nonatomic, copy) NSString *referrerUrl;

/*!
 统计渠道的 URL 地址, 这里有详细的介绍 : 
 https://developers.google.com/analytics/devguides/collection/ios/v2/campaigns#measurement 
 */
@property(nonatomic, copy) NSString *campaignUrl;

/*!
 会话是否开启, 当初始化 tracker 的时候, 此值就会被设置为 true , 当设置为 true , 统计信息在
 发送给服务器的时候, 会加上 "这是会话开始" 的标记. 数据提交完后, 会把此标记清除, 也就是说, 紧跟上来的
 提交, 不会被设置为 true , 因为已经不是会话的开始了 ;-) 
 */
@property(nonatomic, assign) BOOL sessionStart;

/*!
 会话过期时间, 默认情况下为 30 秒, 也就是说, 如果用户没有动作, 30 秒后, 如果继续使用应用的话, 就会
 把 sessionStart 设置为 true, 认为是新会话开始. 
 */
@property(nonatomic, assign) NSTimeInterval sessionTimeout;

/*!
 统计当前屏幕的信息, 屏幕的名称来自于 appScreen , 如果 appScreen 没有设置, 调用此方法不会做
 任何操作, 如果 [GAI optOut] 被设置为 true, 同样不会做任何操作
 注意这里只是收集信息, 存放在本地, 数据提交到服务器使用 [GAI dispatch] 来做
 */
- (BOOL)sendView;

/*!
 This method is deprecated.  See sendView.
 此方法已弃用, 请使用 sendView
 */
- (BOOL)trackView;

/*!
 统计当前屏幕的信息, 屏幕的名称来自于传值进去的 screen 参数. 

 @param screen 当前要统计屏幕的名称
 @return 如果统计信息顺利生成的话返回 `YES` , 如果有错误发生, 返回 'NO' (如:tracker 实例对象错误 )

 注意这里只是收集信息, 存放在本地, 数据提交到服务器使用 [GAI dispatch] 来做
 注意如果 [GAI optOut] 被设置为 true, 这里不会做任何操作. 
 */
- (BOOL)sendView:(NSString *)screen;

/*!
 This method is deprecated.  See sendView.
 此方法已弃用, 请使用 sendView:
 */
- (BOOL)trackView:(NSString *)screen;

/*! 
 统计事件, 参数如下
 @param category 事件的类型, 没有的话 'nil'
 @param action 事件的动作, 没有的话 'nil'
 @param label 事件的标签, 没有的话 'nil'
 @param value 事件的值, 会被转换为 64-bit signed integer , 没有的话 'nil'
 @return 如果统计信息顺利生成的话返回 `YES` , 如果有错误发生, 返回 'NO' (如:tracker 实例对象错误 )
 @example 统计一个按钮被点击了可以使用下面的代码

	tracker sendEventWithCategory:@"uiAction"
	                     withAction:@"buttonPress"
	                      withLabel:buttonName
	                      withValue:[NSNumber numberWithInt:100]];

 注意如果 [GAI optOut] 被设置为 true, 这里不会做任何操作. 
 */
- (BOOL)sendEventWithCategory:(NSString *)category
                   withAction:(NSString *)action
                    withLabel:(NSString *)label
                    withValue:(NSNumber *)value;

/*!
 This method is deprecated. See sendEventWithCategory.
 此方法已弃用, 请使用 sendEventWithCategory:
 */
- (BOOL)trackEventWithCategory:(NSString *)category
                    withAction:(NSString *)action
                     withLabel:(NSString *)label
                     withValue:(NSNumber *)value;

/*! 
 交易统计, 多用于电子商务统计 , 见: 
 https://developers.google.com/analytics/devguides/collection/ios/v2/ecommerce 

 @param transaction GAI 的交易 GAITransaction 对象, 请见 GAITransaction.H 
 @return 如果统计信息顺利生成的话返回 `YES` , 如果有错误发生, 返回 'NO' (如:tracker 实例对象错误 )
 注意如果 [GAI optOut] 被设置为 true, 这里不会做任何操作. 
 */
- (BOOL)sendTransaction:(GAITransaction *)transaction;

/*!
 This method is deprecated. see sendTransaction.
 此方法已弃用, 请使用 sendTransaction:
 */
- (BOOL)trackTransaction:(GAITransaction *)transaction;

/*!
 统计程序异常

 @param isFatal 是否是致命的程序异常
 @param format, ...  程序异常的描述, 格式为 formated nsstring . 
 @return 如果统计信息顺利生成的话返回 `YES` , 如果有错误发生, 返回 'NO' (如:tracker 实例对象错误 )
 @example: 

@try {
  NSArray *highScores = [self getHighScores];
}
@catch (NSException *exception) {
    [tracker sendException:NO // Boolean indicates non-fatal exception.
            withDescription:@"Connection timout %d: %@", connectionError, errorDescription];
}

 注意如果 [GAI optOut] 被设置为 true, 这里不会做任何操作. 
 */
- (BOOL)sendException:(BOOL)isFatal
      withDescription:(NSString *)format, ...;

/*!
 This method is deprecated. See sendException.
 此方法已弃用, 请使用 sendException:
 */
- (BOOL)trackException:(BOOL)isFatal
       withDescription:(NSString *)format, ...;

/*! 
 统计程序异常, 跟上面那个程序差不多, 第二个参数不一样, 方便直接传入 NSException 对象进行统计
 @param isFatal 是否是致命的程序异常
 @param exception 对象类型为 NSException 的程序异常信息
 @return 如果统计信息顺利生成的话返回 `YES` , 如果有错误发生, 返回 'NO' (如:tracker 实例对象错误 )

@example: 

@try {
  NSArray *highScores = [self getHighScores];
}
@catch (NSException *exception) {
  [tracker sendException:NO withNSException:exception);
}

 注意如果 [GAI optOut] 被设置为 true, 这里不会做任何操作. 
 详细见: https://developers.google.com/analytics/devguides/collection/ios/v2/exceptions
 */
- (BOOL)sendException:(BOOL)isFatal
      withNSException:(NSException *)exception;

/*!
 This method is deprecated. See sendException.
 此方法已弃用, 请使用 sendException:
 */
- (BOOL)trackException:(BOOL)isFatal
       withNSException:(NSException *)exception;

/*! 
 统计程序异常, 跟上面那个程序差不多, 第二个参数不一样, 方便直接传入 error 对象进行统计
 @param isFatal 是否是致命的程序异常
 @param exception 对象类型为 NSException 的程序异常信息
 @return 如果统计信息顺利生成的话返回 `YES` , 如果有错误发生, 返回 'NO' (如:tracker 实例对象错误 )

@example: 

NSError *error = nil;
if (![self updateHighScoresWithError:&error]) {
  [tracker sendException:NO withNSError:error);
}

 注意如果 [GAI optOut] 被设置为 true, 这里不会做任何操作. 
 详细见: https://developers.google.com/analytics/devguides/collection/ios/v2/exceptions
 */
- (BOOL)sendException:(BOOL)isFatal
          withNSError:(NSError *)error;

/*!
 This method is deprecated. See sendException.
 此方法已弃用, 请使用 sendException:
 */
- (BOOL)trackException:(BOOL)isFatal
           withNSError:(NSError *)error;

/*! 
 计时器统计, GA 后台查看统计信息入口 -> Engagement -> App Speed 
 @param category 计时器类型
 @param time 所用的时间
 @param name 计时器的名称, 没有的话 'nil'
 @param label 计时器的标记, 没有的话 'nil'
 @return 如果统计信息顺利生成的话返回 `YES` , 如果有错误发生, 返回 'NO' (如:tracker 实例对象错误 )
 @example 假设 onLoad 是在成功加载了 'high scores' 列表后调用, loadTime 是总的加载时间

- (void)onLoad:(NSTimeInterval *)loadTime {
    [tracker sendTimingWithCategory:@"resources"
                            withTimeInterval:loadTime
                                    withName:@"high scores"
                                   withLabel:nil];
    ... // The rest of your onLoad: code.
}

 注意如果 [GAI optOut] 被设置为 true, 这里不会做任何操作. 
 */
- (BOOL)sendTimingWithCategory:(NSString *)category
                     withValue:(NSTimeInterval)time
                      withName:(NSString *)name
                     withLabel:(NSString *)label;

/*!
 This method is deprecated. See sendTimingWithCategory.
 此方法已弃用, 请使用 sendTimingWithCategory:
 */
- (BOOL)trackTimingWithCategory:(NSString *)category
                      withValue:(NSTimeInterval)time
                       withName:(NSString *)name
                      withLabel:(NSString *)label;

/*! 
 统计社交化动作

 @param network 哪个社交网络的 (e.g. Google+, Facebook, Twitter, etc.), 不能为 nil 
 @param action 什么动作 (e.g. Like, Share, +1, etc.), 不能为 nil
 @param target 目标(i.e. a specific 谋篇文章或者是视频), 可以为 nil
 @return 如果统计信息顺利生成的话返回 `YES` , 如果有错误发生, 返回 'NO' (如:tracker 实例对象错误 )
 @example; 

 注意如果 [GAI optOut] 被设置为 true, 这里不会做任何操作. 
 */
- (BOOL)sendSocial:(NSString *)network
        withAction:(NSString *)action
        withTarget:(NSString *)target;

/*!
 This method is deprecated. See sendSocial.
 此方法已弃用, 请使用 sendSocial:
 */
- (BOOL)trackSocial:(NSString *)network
         withAction:(NSString *)action
         withTarget:(NSString *)target;

/*!
 Set a tracking parameter.

 @param parameterName The parameter name.
 @param value The value to set for the parameter. If this is `nil`, the
 value for the parameter will be cleared.

 @returns `YES` if the parameter was set to the given value, or `NO` if there
 was an error (e.g. unknown parameter).
 */
- (BOOL)set:(NSString *)parameterName
      value:(NSString *)value;

/*!
 Get a tracking parameter.

 @param parameterName The parameter name.

 @returns The parameter value, or `nil` if no value for the given parameter is
 set.
 */
- (NSString *)get:(NSString *)parameterName;

/*!
 Queue tracking information with the given parameter values.

 @param trackType The type of tracking information, e.g., @"appview".

 @param parameters A map from parameter names to parameter values which will be
 set just for this piece of tracking information.

 @return `YES` if the tracking information was queued for submission, or `NO`
 if an error occurred (e.g. bad track type).
 */
- (BOOL)send:(NSString *)trackType
      params:(NSDictionary *)parameters;

/*!
 Set a custom dimension value, to be sent at the next tracking call.

 @param index The index at which to set the dimension. Must be positive.

 @param dimension The dimension value, or `nil` if the dimension at the given
 index is to be cleared.

 @return `YES` on success, or `NO` if an error occurred.
 */
- (BOOL)setCustom:(NSInteger)index
        dimension:(NSString *)dimension;

/*!
 Set a custom metric value, to be sent at the next tracking call.

 @param index The index at which to set the metric. Must be positive.

 @param metric The metric value, which will be interpreted as a signed 64-bit
 integer, or `nil` if the metric at the given index is to be cleared.

 @return `YES` on success, or `NO` if an error occurred.
 */
- (BOOL)setCustom:(NSInteger)index
           metric:(NSNumber *)metric;

/*!
 Close the tracker. This will mark it as closed and remove it from the list of
 trackers accessible through [GAI trackerWithTrackingId:], thus decrementing its
 reference count (and causing it to be dealloced unless it has been retained by
 the application). Once this method has been called, it is an error to call any
 of the tracking methods, and they will not result in the generation of any
 tracking information to be submitted to Google Analytics.
 */
- (void)close;

@end
