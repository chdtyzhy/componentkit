/*
 *  Copyright (c) 2014-present, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#import <ComponentKit/CKDefines.h>

#if CK_NOT_SWIFT

#import <Foundation/Foundation.h>

#import <CoreGraphics/CGGeometry.h>

#import <ComponentKit/CKComponentScopeRoot.h>
#import <ComponentKit/CKDataSourceQOS.h>
#import <ComponentKit/CKUpdateMode.h>

@class CKDataSourceChangeset;
@class CKDataSourceConfiguration;
@class CKDataSourceState;

@protocol CKComponentProvider;
@protocol CKComponentStateListener;
@protocol CKDataSourceListener;
@protocol CKDataSourceStateModifying;

@protocol CKDataSourceChangesetModificationGenerator

-(id<CKDataSourceStateModifying>)changesetGenerationModificationForChangeset:(CKDataSourceChangeset *)changeset
                                                                    userInfo:(NSDictionary *)userInfo
                                                                         qos:(CKDataSourceQOS)qos
                                                               stateListener:(id<CKComponentStateListener>)stateListener;


@end

/**
 Describes the currently visible viewport for content rendered by the data source -- this is used to optimize
 component mounting so that components inside the viewport get mounted as soon as possible.
 */
struct CKDataSourceViewport {
  /**
   The size of the viewport. This is equivalent to the scroll view's bounds.
   */
  CGSize size;
  /**
   The point at which the origin of the content view is offset from the origin of the scroll view.
   */
  CGPoint contentOffset;
};

/** Transforms an input of model objects into CKComponentLayouts. All methods and callbacks are main thread only. */
@interface CKDataSource : NSObject <CKComponentStateListener>

/**
 @param configuration @see CKDataSourceConfiguration.
 */
- (instancetype)initWithConfiguration:(CKDataSourceConfiguration *)configuration;

/**
 Applies the specified changes to the data source. If you apply a changeset synchronously while previous asynchronous
 changesets are still pending, they will all be applied synchronously before applying the new changeset.

 @discussion: The default QOS is used on the thread that is processing the application of the changeset.
 */
- (void)applyChangeset:(CKDataSourceChangeset *)changeset
                  mode:(CKUpdateMode)mode
              userInfo:(NSDictionary *)userInfo;

/**
 Applies the specified changes to the data source. If you apply a changeset synchronously while previous asynchronous
 changesets are still pending, they will all be applied synchronously before applying the new changeset.

 @param changeset The new changeset to apply.
 @param mode The mode to use to apply the changeset.
 @param qos The QOS to enforce on the thread applying the modification generated by the new changeset.
 @param userInfo Additional information received with the new changeset.
 */
- (void)applyChangeset:(CKDataSourceChangeset *)changeset
                  mode:(CKUpdateMode)mode
                   qos:(CKDataSourceQOS)qos
              userInfo:(NSDictionary *)userInfo;

/** Updates the configuration object, updating all existing components. */
- (void)updateConfiguration:(CKDataSourceConfiguration *)configuration
                       mode:(CKUpdateMode)mode
                   userInfo:(NSDictionary *)userInfo;

/**
 Regenerate all components in the data source. This can be useful when responding to changes to global singleton state
 that break the "components as a pure function of input" rule (for example, changes to UIAccessibility).
 */
- (void)reloadWithMode:(CKUpdateMode)mode
              userInfo:(NSDictionary *)userInfo;

/**
 Viewport metrics used for calculating items that are in the viewport, when changeset splitting is enabled.
 */
- (void)setViewport:(CKDataSourceViewport)viewport;

- (void)addListener:(id<CKDataSourceListener>)listener;
- (void)removeListener:(id<CKDataSourceListener>)listener;

/*
 Allows the overriding of the generation of a changeset modification. If this is not called it will
 defer to the default behavior.
 */
- (void)setChangesetModificationGenerator:(id<CKDataSourceChangesetModificationGenerator>)changesetModificationGenerator;

@end

#endif
