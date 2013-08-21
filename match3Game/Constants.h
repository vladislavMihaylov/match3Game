//
//  Constants.h
//  match3Test
//
//  Created by Alex Gievsky on 19.06.13.
//  Copyright (c) 2013 reenboog. All rights reserved.
//

#ifndef match3Test_Constants_h
#define match3Test_Constants_h

#define kScreenWidth        1024
#define kScreenHeight       768

#define kNumOfChipTypes     5
#define kFieldBasePoint     ccp(350, 120)
#define kChipSpacing        2
#define kChipWidth          72
#define kChipHeight         72

#define kScorePerChip       10

#define kFieldWidth         8
#define kFieldHeight        8

#define kGameSessionTime    60

#define kFieldAreaWidth (kFieldWidth * kChipWidth + (kFieldWidth - 1) * kChipSpacing)
#define kFieldAreaHeight (kFieldHeight * kChipHeight + (kFieldHeight - 1) * kChipSpacing)

#endif
