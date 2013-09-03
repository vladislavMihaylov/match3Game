//
//  match3GameAppController.h
//  match3Game
//
//  Created by Vlad on 20.08.13.
//  Copyright __MyCompanyName__ 2013. All rights reserved.
//

#import "RootViewController.h"
#include "IOSNDKHelper.h"
#import <Twitter/Twitter.h>
#import <FacebookSDK/FacebookSDK.h>

@implementation RootViewController

//*
 // The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
        // Custom initialization
        [IOSNDKHelper SetNDKReciever:self];
    }
    return self;
}
//*/

- (void) postTwitter:(NSObject *)prms
{
    NSLog(@"post to twitter");
    
    if([SLComposeViewController isAvailableForServiceType:SLServiceTypeTwitter]) {
        
        SLComposeViewController *controller = [SLComposeViewController composeViewControllerForServiceType:SLServiceTypeTwitter];
        
        SLComposeViewControllerCompletionHandler myBlock = ^(SLComposeViewControllerResult result){
                        
            [controller dismissViewControllerAnimated:YES completion:Nil];
        };
        controller.completionHandler =myBlock;
        
        [controller setInitialText:@"Is the best match-3 game ever! "];
        
        [self presentViewController:controller animated:YES completion:Nil];
        
    }
}

- (void) postFacebook:(NSObject *)prms
{
     NSLog(@"post to facebook");
    
//    DEFacebookComposeViewControllerCompletionHandler __block completionHandler = ^(DEFacebookComposeViewControllerResult result) {
//        [self dismissViewControllerAnimated:YES completion:nil];
//        switch (result) {
//            case DEFacebookComposeViewControllerResultCancelled:
//                NSLog(@"Facebook Result: Cancelled");
//                break;
//            case DEFacebookComposeViewControllerResultDone:
//                NSLog(@"Facebook Result: Sent");
//                break;
//        }
//    };
//    DEFacebookComposeViewController * compose = [[DEFacebookComposeViewController alloc] init];
//    [compose setInitialText:self.shareText];
//    [compose addImage:self.shareImage];
//    [compose addURL:self.shareURL];
//    [compose setCompletionHandler:completionHandler];
//    [self presentViewController:compose animated:YES completion:^{}];
    
    if([SLComposeViewController isAvailableForServiceType:SLServiceTypeFacebook]) {
        
        SLComposeViewController *controller = [SLComposeViewController composeViewControllerForServiceType:SLServiceTypeFacebook];
        
        SLComposeViewControllerCompletionHandler myBlock = ^(SLComposeViewControllerResult result){
            
            [controller dismissViewControllerAnimated:YES completion:Nil];
        };
        controller.completionHandler =myBlock;
        
        [controller setInitialText:@"Is the best match-3 game ever! "];
        
        [self presentViewController:controller animated:YES completion:Nil];
        
    }
    
}


/*
// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
}
*/

/*
// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
}
 
*/
// Override to allow orientations other than the default portrait orientation.
// This method is deprecated on ios6
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    return UIInterfaceOrientationIsLandscape( interfaceOrientation );
}

// For ios6, use supportedInterfaceOrientations & shouldAutorotate instead
- (NSUInteger) supportedInterfaceOrientations{
#ifdef __IPHONE_6_0
    return UIInterfaceOrientationMaskLandscape;
#endif
}

- (BOOL) shouldAutorotate {
    return YES;
}

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}


- (void)dealloc {
    [super dealloc];
}


@end
