#include "TextTests.h"
#include "Tests.h"
#include "Internationalization/Text.h"

void TextTestsRun()
{

// Write all test here

TEST_CASE(TestTextIterCopy)
{
   // "privet", in cyrillic
   unsigned char data[] = { 0xd0, 0xbf, 0xd1, 0x80, 0xd0, 0xb8, 0xd0,
                  0xb2, 0xd0, 0xb5, 0xd1, 0x82 };
   totem::Text text(data, sizeof(data));

   totem::Text::Iterator iter(text);
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
   totem::Text::Iterator iter2(text);
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
TEST_END(TestTextIterCopy)

TEST_CASE(TestTextIterOnInvalidContinuationByte)
{
   /* the second byte should be a continuation byte
    * but it is not
    */
   unsigned char data[] = { 0xd0, 0x7f /*invalid cont byte*/,
                              0xd1, 0x80, 0xd0, 0xb8, 0xd0,
                              0xb2, 0xd0, 0xb5, 0xd1, 0x82 };
   totem::Text text(data, sizeof(data));
   totem::Text::Iterator iter(text);
   // Should not even get first codepoint, cause its cont byte is broken
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
   totem::Text::Iterator iter1(text1);
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
TEST_END(TestTextIterOnInvalidContinuationByte)

TEST_CASE(TestTextIterOnInvalidFirstByte)
{
   /* the first byte cannot start with 1010xxxx in UTF-8
    */
   unsigned char data[] = { 0xa0 /* invalid firs byte */, 0x7f,
                              0xd1, 0x80, 0xd0, 0xb8, 0xd0,
                              0xb2, 0xd0, 0xb5, 0xd1, 0x82 };
   totem::Text text(data, sizeof(data));
   totem::Text::Iterator iter(text);
   // So iterator should instantly be in ended state
   TEST_ASSERT(iter.HasEnded());
}
TEST_END(TestTextIterOnInvalidFirstByte)

TEST_CASE(TestTextIterOnTrivialData)
{
   totem::Text text(nullptr, 0);
   totem::Text::Iterator iter(text);
   TEST_ASSERT(iter.HasEnded());
   totem::unicode_t u = iter.Get();
   TEST_ASSERT(u == 0x0);
   iter.Next();
   TEST_ASSERT(iter.HasEnded());
   TEST_ASSERT(u == 0x0);

   totem::Text text1;
   totem::Text::Iterator iter1(text1);
   TEST_ASSERT(iter1.HasEnded());
   u = iter1.Get();
   TEST_ASSERT(u == 0x0);
   iter1.Next();
   TEST_ASSERT(iter1.HasEnded());
   TEST_ASSERT(u == 0x0);
}
TEST_END(TestTextIterOnTrivialData)

TEST_CASE(TestTextIterOnNonTrivialData)
{
   // "privet", in cyrillic
   unsigned char data[] = { 0xd0, 0xbf, 0xd1, 0x80, 0xd0, 0xb8, 0xd0,
                  0xb2, 0xd0, 0xb5, 0xd1, 0x82 };
   totem::Text text(data, sizeof(data));

   totem::Text::Iterator iter(text);
   totem::unicode_t u = iter.Get();
   TEST_ASSERT(u == 0x43F); // Should be cyrillic p
   iter.Next();
   TEST_ASSERT(!iter.HasEnded());
   u = iter.Get();
   TEST_ASSERT(u == 0x440) // Should be cyrillic r
   iter.Next();
   TEST_ASSERT(!iter.HasEnded());
   u = iter.Get();
   TEST_ASSERT(u == 0x438); // Should be cyrillic i
   iter.Next();
   iter.Next();
   iter.Next();
   TEST_ASSERT(!iter.HasEnded());
   iter.Next();
   TEST_ASSERT(iter.HasEnded());

   totem::Text text1(data, 2);
   totem::Text::Iterator iter1(text1);
   u = iter1.Get();
   TEST_ASSERT(u == 0x43F); // Should be cyrillic p
   TEST_ASSERT(!iter1.HasEnded());
   iter1.Next();
   TEST_ASSERT(iter1.HasEnded());
}
TEST_END(TestTextIterOnNonTrivialData)

TEST_CASE(TestSubTextConstructionAndIteration)
{
   // "privet", in cyrillic
   unsigned char data[] = { 0xd0, 0xbf, 0xd1, 0x80, 0xd0, 0xb8, 0xd0,
                  0xb2, 0xd0, 0xb5, 0xd1, 0x82 };
   totem::Text text(data, sizeof(data));

   totem::Text::SubText subText(text, 1, 3); // Should be "riv"
   totem::Text::Iterator iter(subText);

   totem::unicode_t u = iter.Get();
   TEST_ASSERT(u == 0x440) // Should be cyrillic r
   iter.Next();
   TEST_ASSERT(!iter.HasEnded());
   u = iter.Get();
   TEST_ASSERT(u == 0x438) // Should be cyrillic i
   iter.Next();
   TEST_ASSERT(!iter.HasEnded());
   u = iter.Get();
   TEST_ASSERT(u == 0x432) // Should be cyrillic v
   iter.Next();
   TEST_ASSERT(iter.HasEnded());

   totem::Text::SubText subText1(text, -3, 3); // Should be "vet"
   totem::Text::Iterator iter1(subText1);

   u = iter1.Get();
   TEST_ASSERT(u == 0x432) // Should be cyrillic v
   iter1.Next();
   TEST_ASSERT(!iter1.HasEnded());
   u = iter1.Get();
   TEST_ASSERT(u == 0x435) // Should be cyrillic e
   iter1.Next();
   TEST_ASSERT(!iter1.HasEnded());
   u = iter1.Get();
   TEST_ASSERT(u == 0x442) // Should be cyrillic v
   iter1.Next();
   TEST_ASSERT(iter1.HasEnded());
}
TEST_END(TestSubTextConstructionAndIteration)

TEST_CASE(TestConstSubTextConstructionAndIteration)
{
   // "privet", in cyrillic
   const unsigned char data[] = { 0xd0, 0xbf, 0xd1, 0x80, 0xd0, 0xb8, 0xd0,
                  0xb2, 0xd0, 0xb5, 0xd1, 0x82 };
   const totem::Text text(data, sizeof(data));

   totem::Text::ConstSubText subText(text, 1, 3); // Should be "riv"
   totem::Text::Iterator iter(subText);

   totem::unicode_t u = iter.Get();
   TEST_ASSERT(u == 0x440) // Should be cyrillic r
   iter.Next();
   TEST_ASSERT(!iter.HasEnded());
   u = iter.Get();
   TEST_ASSERT(u == 0x438) // Should be cyrillic i
   iter.Next();
   TEST_ASSERT(!iter.HasEnded());
   u = iter.Get();
   TEST_ASSERT(u == 0x432) // Should be cyrillic v
   iter.Next();
   TEST_ASSERT(iter.HasEnded());

   totem::Text::ConstSubText subText1(text, -3, 3); // Should be "vet"
   totem::Text::Iterator iter1(subText1);

   u = iter1.Get();
   TEST_ASSERT(u == 0x432) // Should be cyrillic v
   iter1.Next();
   TEST_ASSERT(!iter1.HasEnded());
   u = iter1.Get();
   TEST_ASSERT(u == 0x435) // Should be cyrillic e
   iter1.Next();
   TEST_ASSERT(!iter1.HasEnded());
   u = iter1.Get();
   TEST_ASSERT(u == 0x442) // Should be cyrillic t
   iter1.Next();
   TEST_ASSERT(iter1.HasEnded());
}
TEST_END(TestConstSubTextConstructionAndIteration)

TEST_CASE(TestTextGetWordsCyrillicOneSpace)
{
   // "Privet, kak tvoi dela?" in cyrillic
   const unsigned char data[] = {
      0xd0, 0x9f, 0xd1, 0x80, 0xd0, 0xb8, 0xd0, 0xb2, 0xd0, 0xb5,
      0xd1, 0x82, 0x2c, 0x20, 0xd0, 0xba, 0xd0, 0xb0, 0xd0, 0xba,
      0x20, 0xd1, 0x82, 0xd0, 0xb2, 0xd0, 0xbe, 0xd0, 0xb8, 0x20,
      0xd0, 0xb4, 0xd0, 0xb5, 0xd0, 0xbb, 0xd0, 0xb0, 0x3f
   };

   const totem::Text text(data, sizeof(data));

   totem::Text::ConstSubText first(text, 0, 7);
   totem::Text::ConstSubText second(text, 8, 3);
   totem::Text::ConstSubText third(text, 12, 4);
   totem::Text::ConstSubText fourth(text, 17, 5);

   totem::Text::ConstSubTextVec words = text.GetWords();
   TEST_ASSERT(words.GetCount() == 4);
   if(words.GetCount() != 4)
      return;

   TEST_ASSERT(words[0] == first);
   TEST_ASSERT(words[1] == second);
   TEST_ASSERT(words[2] == third);
   TEST_ASSERT(words[3] == fourth);
}
TEST_END(TestTextGetWordsCyrillicOneSpace)

TEST_CASE(TestTextConcatination)
{
   // "Privet, kak tvoi dela?" in cyrillic
   // Length: 22
   const unsigned char data1[] = {
      0xd0, 0x9f, 0xd1, 0x80, 0xd0, 0xb8, 0xd0, 0xb2, 0xd0, 0xb5,
      0xd1, 0x82, 0x2c, 0x20, 0xd0, 0xba, 0xd0, 0xb0, 0xd0, 0xba,
      0x20, 0xd1, 0x82, 0xd0, 0xb2, 0xd0, 0xbe, 0xd0, 0xb8, 0x20,
      0xd0, 0xb4, 0xd0, 0xb5, 0xd0, 0xbb, 0xd0, 0xb0, 0x3f
   };

   // "Privet", in cyrillic
   // Length: 6
   const unsigned char data2[] = {
      0xd0, 0xbf, 0xd1, 0x80, 0xd0, 0xb8, 0xd0,
                  0xb2, 0xd0, 0xb5, 0xd1, 0x82 
   };

   // Test for operator +
   totem::Text text1(data1, sizeof(data1));
   totem::Text text2(data2, sizeof(data2));
   int resLength = text1.GetLength() + text2.GetLength();
   totem::Text text3 = text1 + text2;
   totem::Text::ConstSubText subText1(text3, 0, 22);
   totem::Text::ConstSubText subText2(text3, 22, 6);
   TEST_ASSERT(text3.GetLength() == resLength);
   TEST_ASSERT(subText1 == text1);
   TEST_ASSERT(subText2 == text2);

   // Test for operator += with available space for text2
   totem::Text textBig(500);
   textBig += text1;
   textBig += text2;
   totem::Text::ConstSubText subText3(textBig, 0, 22);
   totem::Text::ConstSubText subText4(textBig, 22, 6);
   TEST_ASSERT(textBig.GetLength() == resLength);
   TEST_ASSERT(subText3 == text1);
   TEST_ASSERT(subText4 == text2);

   // Test for operator += with NO available space for text2
   totem::Text savedText = text1;
   text1 += text2;
   totem::Text::ConstSubText subText5(text1, 0, 22);
   totem::Text::ConstSubText subText6(text1, 22, 6);
   TEST_ASSERT(text1.GetLength() == resLength);
   TEST_ASSERT(subText5 == savedText);
   TEST_ASSERT(subText6 == text2);
}
TEST_END(TestTextConcatination)

TEST_CASE(TestSubTextGetText)
{
   // "Privet, kak tvoi dela?" in cyrillic
   // Length: 22
   const unsigned char data[] = {
      0xd0, 0x9f, 0xd1, 0x80, 0xd0, 0xb8, 0xd0, 0xb2, 0xd0, 0xb5,
      0xd1, 0x82, 0x2c, 0x20, 0xd0, 0xba, 0xd0, 0xb0, 0xd0, 0xba,
      0x20, 0xd1, 0x82, 0xd0, 0xb2, 0xd0, 0xbe, 0xd0, 0xb8, 0x20,
      0xd0, 0xb4, 0xd0, 0xb5, 0xd0, 0xbb, 0xd0, 0xb0, 0x3f
   };

   totem::Text text(data, sizeof(data));
   totem::Text::ConstSubText subText(text, 5, 10);
   totem::Text text1 = subText.GetText();
   TEST_ASSERT(text1 == subText); 
}
TEST_END(TestSubTextGetText)

// End of tests
}
