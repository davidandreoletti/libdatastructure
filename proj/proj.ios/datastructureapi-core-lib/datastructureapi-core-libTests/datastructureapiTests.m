//
//  datastructureapiTests.m
//
//  Created by David Andreoletti on 8/24/12.
//  Copyright (c) 2012 IO Stark. All rights reserved.
//

#import "datastructureapiTests.h"
#import "TestRunner.h"

@implementation datastructureapiTests

- (void)setUp
{
    [super setUp];
    
    // Set-up code here.
}

- (void)tearDown
{
    // Tear-down code here.
    
    [super tearDown];
   
    // Prevent Xcode from reporting Tests did not finish on fast test suites
    [NSThread sleepForTimeInterval:5.0];
}

- (void)testLoggingMasterTestSuite
{
    int r = (run_unit_tests_all() == 1);
    STAssertEquals(1, r , @"Master Test Suite Failed.");

}

@end
