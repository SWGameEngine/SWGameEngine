//
//  GameView.m
//  SWGameEngine
//
//  Created by James Chen on 10/25/16.
//  Copyright © 2016 James Chen. All rights reserved.
//

#import "GameView.h"

@implementation GameView

-(void)awakeFromNib
{
    NSLog(@"awakeFromNib ...");

    [super awakeFromNib];

    [NSTimer scheduledTimerWithTimeInterval:0.016f
                                     target:self
                                   selector:@selector(onTick:)
                                   userInfo:nil
                                    repeats:YES];
}

-(void)onTick:(NSTimer *)timer {
    [self setNeedsDisplay:TRUE];
}

- (void)drawRect:(NSRect)dirtyRect {
    [super drawRect:dirtyRect];
    
    // Drawing code here.
//    NSLog(@"drawRect: %f, %f, %f, %f", dirtyRect.origin.x, dirtyRect.origin.y,
//          dirtyRect.size.width, dirtyRect.size.height);

    NSRect bounds = [self bounds];
    CGFloat r = arc4random() % 255 / 255.0f;
    CGFloat g = arc4random() % 255 / 255.0f;
    CGFloat b = arc4random() % 255 / 255.0f;
    [[NSColor colorWithRed:r green:g blue:b alpha:1.0f] set];
    [NSBezierPath fillRect:bounds];

}

@end
