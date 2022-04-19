#include "TextTests.h"
#include "Tests.h"
#include "Internationalization/Text.h"

void TestTextIterCopy()
{
   // Privet, in cyrillic
   unsigned char data[] = { 0xd0, 0xbf, 0xd1, 0x80, 0xd0, 0xb8, 0xd0,
                  0xb2, 0xd0, 0xb5, 0xd1, 0x82 };
   totem::Text text(data, sizeof(data));

   totem::Text::Iterator iter(&text);
   totem::Text::Iterator iter1(iter);
   totem::unicode_t u = iter.Get();
   totem::unicode_t u1 = iter1.Get();
   TEST_ASSERT(u == 0x43F); // Cyrillic p
   TEST_ASSERT(u == u1);
   iter.Next();
   u = iter.Get();
   u1 = iter1.Get();
   TEST_ASSERT(u == 0x440); // Cyrillic r
   TEST_ASSERT(u1 == 0x43F); // Should stay the same
   totem::Text::Iterator iter2(&text);
   iter2.Next();
   iter2 = iter1;
   totem::unicode_t u2 = iter2.Get();
   TEST_ASSERT(u2 == 0x43F);
   iter1.Next();
   iter2.Next();
   iter2.Next();
   u1 = iter1.Get();
   u2 = iter2.Get();
   TEST_ASSERT(u1 == 0x440) // Cyrillic r
   TEST_ASSERT(u2 == 0x438) // Cyrillic i
}

void TestTextIterOnInvalidContinuationByte()
{
   /* the second byte should be a continuation byte
    * but it is not
    */
   unsigned char data[] = { 0xd0, 0x7f /*invalid cont byte*/,
                              0xd1, 0x80, 0xd0, 0xb8, 0xd0,
                              0xb2, 0xd0, 0xb5, 0xd1, 0x82 };
   totem::Text text(data, sizeof(data));
   totem::Text::Iterator iter(&text);
   // Cannot even get first codepoint, cause its cont byte is broken
   TEST_ASSERT(iter.HasEnded());
   totem::unicode_t u = iter.Get();
   TEST_ASSERT(u == 0x0);


   /* the fourth byte should be a continuation byte
    * but it is not
    */
   unsigned char data1[] = { 0xd0, 0xbf, 0xd1,
                              0x70 /* invalid cont byte */,
                              0xd0, 0xb8, 0xd0, 0xb2, 0xd0, 0xb5,
                              0xd1, 0x82 };
   totem::Text text1(data1, sizeof(data1));
   totem::Text::Iterator iter1(&text1);
   // First codepoint is now OK
   TEST_ASSERT(!iter1.HasEnded());
   u = iter1.Get();
   TEST_ASSERT(u == 0x43F); // Cyrillic p
   iter1.Next();
   // The second codepoint is broken
   TEST_ASSERT(iter1.HasEnded());
   u = iter1.Get();
   TEST_ASSERT(u == 0x0);
}


void TestTextIterOnInvalidFirstByte()
{
   /* the first byte cannot start with 1010xxxx in UTF-8
    */
   unsigned char data[] = { 0xa0 /* invalid firs byte */, 0x7f,
                              0xd1, 0x80, 0xd0, 0xb8, 0xd0,
                              0xb2, 0xd0, 0xb5, 0xd1, 0x82 };
   totem::Text text(data, sizeof(data));
   totem::Text::Iterator iter(&text);
   // So iterator should instantly be in ended state
   TEST_ASSERT(iter.HasEnded());
}

void TestTextIterOnTrivialData()
{
   totem::Text text(nullptr, 0);
   totem::Text::Iterator iter(&text);
   TEST_ASSERT(iter.HasEnded());
   totem::unicode_t u = iter.Get();
   TEST_ASSERT(u == 0x0);
   iter.Next();
   TEST_ASSERT(iter.HasEnded());
   TEST_ASSERT(u == 0x0);
}

void TestTextIterOnNonTrivialData()
{
   // Privet, in cyrillic
   unsigned char data[] = { 0xd0, 0xbf, 0xd1, 0x80, 0xd0, 0xb8, 0xd0,
                  0xb2, 0xd0, 0xb5, 0xd1, 0x82 };
   totem::Text text(data, sizeof(data));

   totem::Text::Iterator iter(&text);
   totem::unicode_t u = iter.Get();
   TEST_ASSERT(u == 0x43F) // Should be cyrillic p
   iter.Next();
   TEST_ASSERT(!iter.HasEnded());
   u = iter.Get();
   TEST_ASSERT(u == 0x440) // Should be cyrillic r
   iter.Next();
   TEST_ASSERT(!iter.HasEnded());
   u = iter.Get();
   TEST_ASSERT(u == 0x438) // Should be cyrillic i
   iter.Next();
   iter.Next();
   iter.Next();
   TEST_ASSERT(!iter.HasEnded());
   iter.Next();
   TEST_ASSERT(iter.HasEnded());
}


void TextTestsRun()
{
   TestTextIterOnNonTrivialData();
   TestTextIterOnTrivialData();
   TestTextIterOnInvalidContinuationByte();
   TestTextIterOnInvalidFirstByte();
   TestTextIterCopy();
}
