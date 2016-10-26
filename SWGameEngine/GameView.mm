//
//  GameView.m
//  SWGameEngine
//
//  Created by James Chen on 10/25/16.
//  Copyright © 2016 James Chen. All rights reserved.
//

#import "GameView.h"
#import <AppKit/AppKit.h>

#include "Game.hpp"

#define kDrawingColor ([NSColor yellowColor].CGColor)
#define kLineWeight (1.5)

@implementation GameView

@synthesize context = _context, drawingLayer = _drawingLayer;

-(void)awakeFromNib
{
    NSLog(@"awakeFromNib, fameSize: %f, %f", self.frame.size.width, self.frame.size.height);

    [super awakeFromNib];

    // this is total boilerplate, it rarely needs to change
    CGColorSpaceRef colorspace = CGColorSpaceCreateDeviceRGB();
    CGFloat width = self.frame.size.width;
    CGFloat height = self.frame.size.height;
    size_t bitsPerComponent = 8;
    size_t bytesPerRow = (4 * width);

    int* data = (int*) malloc(width * height * sizeof(int));
    for (int i = 0, len = width * height; i < len; ++i)
    {
        data[i] = 0xFF0000FF; // BGRA format
    }

    self.context = CGBitmapContextCreate(data, width, height, bitsPerComponent, bytesPerRow, colorspace, kCGImageAlphaPremultipliedFirst);

    CGColorSpaceRelease(colorspace);

    CGSize size = self.frame.size;
    self.drawingLayer = CGLayerCreateWithContext(self.context, size, NULL);

    Game::getInstance()->init((byte*)CGBitmapContextGetData(self.context), width, height);

    [NSTimer scheduledTimerWithTimeInterval:0.016f
                                     target:self
                                   selector:@selector(onTick:)
                                   userInfo:nil
                                    repeats:YES];
}

-(void)onTick:(NSTimer *)timer {
    [self plotPoint:CGPointMake(100, 100)];
}

- (void)drawRect:(NSRect)dirtyRect {
    [super drawRect:dirtyRect];
    
    // Drawing code here.
//    NSLog(@"drawRect: %f, %f, %f, %f", dirtyRect.origin.x, dirtyRect.origin.y,
//          dirtyRect.size.width, dirtyRect.size.height);

//    NSRect bounds = [self bounds];
//    CGFloat r = arc4random() % 255 / 255.0f;
//    CGFloat g = arc4random() % 255 / 255.0f;
//    CGFloat b = arc4random() % 255 / 255.0f;
//    [[NSColor colorWithRed:r green:g blue:b alpha:1.0f] set];
//    [NSBezierPath fillRect:bounds];

    // this creates a new blank image, then gets the surface you've drawn on, and stamps it down
    // at some point, the hardware will render this onto the screen
    CGContextRef currentContext = (CGContextRef)[[NSGraphicsContext currentContext] graphicsPort];//UIGraphicsGetCurrentContext();

//    CGContextSetRGBFillColor (currentContext, 1, 0, 0, 1);// 3
//    CGContextFillRect (currentContext, CGRectMake (0, 0, 200, 100 ));// 4
//    CGContextSetRGBFillColor (currentContext, 0, 0, 1, .5);// 5
//    CGContextFillRect (currentContext, CGRectMake (0, 0, 100, 200));

//    int* data = (int*)CGBitmapContextGetData(self.context);
//    NSLog(@"drawRect: %p", data);
//    int width = self.frame.size.width;
//    int height = self.frame.size.height;
//
//    for (int i = 0, len = width * height; i < len; ++i)
//    {
//        data[i] = 0x0000FFFF; // ARGB format
//    }

    Game::getInstance()->render();

    CGImageRef image = CGBitmapContextCreateImage(self.context);
    CGRect bounds = [self bounds];
    CGContextDrawImage(currentContext, bounds, image);
    CGImageRelease(image);
    CGContextDrawLayerInRect(currentContext, bounds, self.drawingLayer);
}

- (void) plotPoint:(CGPoint) point; {

//    CGContextRef layerContext = CGLayerGetContext(self.drawingLayer); // get ready to draw on your drawing surface
//
//    // prepare to draw
//    CGContextSetLineWidth(layerContext, kLineWeight);
//    CGContextSetLineCap(layerContext, kCGLineCapRound);
//    CGContextSetStrokeColorWithColor(layerContext, kDrawingColor);
//
//    // draw onto surface by building a path, then stroking it
//    CGContextBeginPath(layerContext); // start
//
//    CGFloat x = point.x;
//    CGFloat y = point.y;
//    CGContextMoveToPoint(layerContext, x, y);
//    CGContextAddLineToPoint(layerContext, x, y);
//
//    CGContextStrokePath(layerContext); // finish

    [self setNeedsDisplay:TRUE]; // this tells system to call drawRect at a time of it's choosing
}

@end
