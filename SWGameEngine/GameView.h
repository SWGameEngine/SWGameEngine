//
//  GameView.h
//  SWGameEngine
//
//  Created by James Chen on 10/25/16.
//  Copyright © 2016 James Chen. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <CoreGraphics/CoreGraphics.h>

@interface GameView : NSView {

@public
    CGContextRef _context;
    CGLayerRef _drawingLayer;
}

@property (nonatomic) CGContextRef context;
@property (nonatomic) CGLayerRef drawingLayer; // this is the drawing surface

@end
