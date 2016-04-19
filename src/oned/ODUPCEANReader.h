#pragma once
/*
* Copyright 2016 ZXing authors
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include "oned/ODReader.h"
#include "ZXString.h"

#include <vector>

namespace ZXing {

enum class BarcodeFormat;
enum class ErrorStatus;

namespace OneD {

/**
* <p>Encapsulates functionality and implementation that is common to UPC and EAN families
* of one-dimensional barcodes.</p>
*/
class UPCEANReader : public Reader
{
public:
	virtual Result decodeRow(int rowNumber, const BitArray& row, const DecodeHints* hints) const override;

protected:
	/**
	* <p>Like {@link #decodeRow(int, BitArray, java.util.Map)}, but
	* allows caller to inform method about where the UPC/EAN start pattern is
	* found. This allows this to be computed once and reused across many implementations.</p>
	*
	* @param rowNumber row index into the image
	* @param row encoding of the row of the barcode image
	* @param startGuardRange start/end column where the opening start pattern was found
	* @param hints optional hints that influence decoding
	* @return {@link Result} encapsulating the result of decoding a barcode in the row
	* @throws NotFoundException if no potential barcode is found
	* @throws ChecksumException if a potential barcode is found but does not pass its checksum
	* @throws FormatException if a potential barcode is found but format is invalid
	*/
	virtual Result decodeRow(int rowNumber, const BitArray& row, int startGuardBegin, int startGuardEnd, const DecodeHints* hints) const;


	/**
	* Get the format of this decoder.
	*/
	virtual BarcodeFormat supportedFormat() const = 0;

	/**
	* Subclasses override this to decode the portion of a barcode between the start
	* and end guard patterns.
	*
	* @param row row of black/white values to search
	* @param startRange start/end offset of start guard pattern
	* @param resultString {@link StringBuilder} to append decoded chars to
	* @return horizontal offset of first pixel after the "middle" that was decoded
	* @throws NotFoundException if decoding could not complete successfully
	*/
	virtual ErrorStatus decodeMiddle(const BitArray& row, int startGuardBegin, int startGuardEnd, int &resultOffset, String& resultString) const = 0;

	/**
	* @param s string of digits to check
	* @return {@link #checkStandardUPCEANChecksum(CharSequence)}
	* @throws FormatException if the string does not contain only digits
	*/
	virtual	ErrorStatus checkChecksum(const String& s) const;


	virtual ErrorStatus decodeEnd(const BitArray& row, int endStart, int& begin, int& end) const;

public:
	static ErrorStatus FindStartGuardPattern(const BitArray& row, int& begin, int& end);
	static ErrorStatus FindGuardPattern(const BitArray& row, int rowOffset, bool whiteFirst, const std::vector<int>& pattern, int& begin, int& end);

	/**
	* Computes the UPC/EAN checksum on a string of digits, and reports
	* whether the checksum is correct or not.
	*
	* @param s string of digits to check
	* @return true iff string of digits passes the UPC/EAN checksum algorithm
	* @throws FormatException if the string does not contain only digits
	*/
	static ErrorStatus CheckStandardUPCEANChecksum(const String& s);

	/**
	* Attempts to decode a single UPC/EAN-encoded digit.
	*
	* @param row row of black/white values to decode
	* @param counters the counts of runs of observed black/white/black/... values
	* @param rowOffset horizontal offset to start decoding from
	* @param patterns the set of patterns to use to decode -- sometimes different encodings
	* for the digits 0-9 are used, and this indicates the encodings for 0 to 9 that should
	* be used
	* @return horizontal offset of first pixel beyond the decoded digit
	* @throws NotFoundException if digit cannot be decoded
	*/
	static ErrorStatus DecodeDigit(const BitArray& row, int rowOffset, const std::vector<std::vector<int>>& patterns, std::vector<int>& counters, int &resultOffset);
	
	/**
	* Start/end guard pattern.
	*/
	static const std::vector<int> START_END_PATTERN;

	/**
	* Pattern marking the middle of a UPC/EAN pattern, separating the two halves.
	*/
	static const std::vector<int> MIDDLE_PATTERN;
	/**
	* end guard pattern.
	*/
	static const std::vector<int> END_PATTERN;
	
	/**
	* "Odd", or "L" patterns used to encode UPC/EAN digits.
	*/
	static const std::vector<std::vector<int>> L_PATTERNS;

	/**
	* As above but also including the "even", or "G" patterns used to encode UPC/EAN digits.
	*/
	static const std::vector<std::vector<int>> L_AND_G_PATTERNS;
};

} // OneD
} // ZXing
