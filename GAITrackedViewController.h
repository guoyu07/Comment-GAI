/*!
 @header    GAITrackedViewController.h
 @abstract  Google Analytics for iOS Tracked View Controller Header
 @version   2.0
 @copyright Copyright 2012 Google Inc. All rights reserved.
 */

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@protocol GAITracker;

/*!
 Extends UIViewController to generate Google Analytics view tracking calls
 whenever the view appears; this is done by overriding the `viewDidAppear:`
 method. The view name must be set for any tracking calls to be made.

 By default, this will use [GAI defaultTracker] for tracking calls, but one can
 override this by setting the tracker property.

 此类是对 UIViewController 的扩展, 加入了 GAI 统计实现, 让遵循了 MVC 开发原则的 APP 集成 GAI 
 更方便. 见下面例子: 

如果你的 '关于我们' 页面是这么实现的 (继承了 UIViewController )
@interface AboutViewController : UIViewController 

那么你可以这样写 
#import "GAITrackedViewController.h"
@interface AboutViewController : GAITrackedViewController

然后在 .m 文件里面的 viewDidLoad 写上 trackedViewName 
- (void)viewDidLoad {
  [super viewDidLoad];
  self.trackedViewName = @"关于我们";
}
 */
@interface GAITrackedViewController : UIViewController {
 @private
  id<GAITracker>  tracker_;
  NSString        *trackedViewName_;
}

/*!
 The tracker on which view tracking calls are be made, or `nil`, in which case
 [GAI defaultTracker] will be used.
 */
@property(nonatomic, assign) id<GAITracker> tracker;
/*!
 The view name, for purposes of Google Analytics view tracking. If this is
 `nil`, no tracking calls will be made.
 
 统计当前屏幕的名称, 相对于 web 版本的 GA 统计里面, 网页的 TITLE 标签
 */
@property(nonatomic, copy)   NSString *trackedViewName;

@end
