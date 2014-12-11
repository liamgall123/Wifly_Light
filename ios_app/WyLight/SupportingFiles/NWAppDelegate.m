//
//  NWAppDelegate.m
//  WyLight
//
//  Created by Nils Weiß on 19.09.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#import "NWAppDelegate.h"
#import <CoreData/CoreData.h>
#import "Script+serialization.h"

@implementation NWAppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    // Override point for customization after application launch.
    return YES;
}
							
- (void)applicationWillResignActive:(UIApplication *)application
{
	// Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
	// Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
	// Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
	// If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
	// Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
	// Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
}

- (void)applicationWillTerminate:(UIApplication *)application
{
	// Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
}

- (BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation {
	if (url){
		NSURL *documentUrl = [[[NSFileManager defaultManager] URLsForDirectory:NSDocumentDirectory inDomains:NSUserDomainMask] lastObject];
		documentUrl = [documentUrl URLByAppendingPathComponent:@"ScriptDocument"];
		UIManagedDocument *document = [[UIManagedDocument alloc] initWithFileURL:documentUrl];
		
		if (![[NSFileManager defaultManager] fileExistsAtPath:[documentUrl path]]) {
			[document saveToURL:documentUrl forSaveOperation:UIDocumentSaveForCreating completionHandler:^(BOOL success) {
				if (success) {
					[Script deserializeScriptFromPath:url inContext:document.managedObjectContext];
					[[[UIAlertView alloc] initWithTitle:NSLocalizedStringFromTable(@"ImportScriptKey", @"ViewControllerLocalization", @"")
												message:nil delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil, nil] show];
					NSError *error;
					if (![document.managedObjectContext save:&error]) {
						NSLog(@"save error");
					}
				} else {
					NSLog(@"saveToURL failed");
				}
			}];
		} else if (document.documentState == UIDocumentStateClosed) {
			[document openWithCompletionHandler:^(BOOL success) {
				if (success) {
					[Script deserializeScriptFromPath:url  inContext:document.managedObjectContext];
					[[[UIAlertView alloc] initWithTitle:NSLocalizedStringFromTable(@"ImportScriptKey", @"ViewControllerLocalization", @"")
												message:nil delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil, nil] show];
					NSError *error;
					if (![document.managedObjectContext save:&error]) {
						NSLog(@"save error");
					}
				}
			}];
		} else {
			[Script deserializeScriptFromPath:url  inContext:document.managedObjectContext];
			[[[UIAlertView alloc] initWithTitle:NSLocalizedStringFromTable(@"ImportScriptKey", @"ViewControllerLocalization", @"")
										message:nil delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil, nil] show];
			NSError *error;
			if (![document.managedObjectContext save:&error]) {
				NSLog(@"save error");
			}
		}
	}
	return YES;
}

@end