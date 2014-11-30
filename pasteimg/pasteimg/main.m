//
//  main.m
//  pasteimage
//  cli app for extracting image from clipboard and sending it to stdout as binary data
//  compile with: gcc main.m -o pbpaste-image -ObjC -framework AppKit

#import <AppKit/AppKit.h>
#import <Foundation/Foundation.h>

int main(int argc, const char * argv[]) {
    @autoreleasepool {
        NSPasteboard *pasteBoard = [NSPasteboard generalPasteboard];
        NSImage *image = [[NSImage alloc] initWithPasteboard:pasteBoard];
        NSData *data = [NSBitmapImageRep
                        representationOfImageRepsInArray:[image representations]
                        usingType:NSPNGFileType
                        properties:nil];
        
        NSFileHandle *handler = [NSFileHandle fileHandleWithStandardOutput];
        [handler writeData:data];
    }
    return 0;
}
