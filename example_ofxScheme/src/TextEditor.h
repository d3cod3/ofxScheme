#pragma once

#include <string>
#include <vector>
#include <array>
#include <memory>
#include <unordered_set>
#include <unordered_map>
#include <map>
#if defined(MAC_OS_X_VERSION_10_12)
    #undef tolower // needed for pyport.h not to conflict with regex on osx 10.12
#endif
#include <regex>
#include "imgui.h"

class IMGUI_API TextEditor
{
public:
	enum class PaletteIndex
	{
		Default,
		Keyword,
		Number,
		String,
		CharLiteral,
		Punctuation,
		Preprocessor,
		Identifier,
		KnownIdentifier,
		PreprocIdentifier,
		Comment,
		MultiLineComment,
		Background,
		Cursor,
		Selection,
		ErrorMarker,
		Breakpoint,
		LineNumber,
		CurrentLineFill,
		CurrentLineFillInactive,
		CurrentLineEdge,
		Max
	};

	enum class SelectionMode
	{
		Normal,
		Word,
		Line
	};

	struct Breakpoint
	{
        bool mEnabled;
        int mLine;
        std::string mCondition;

		Breakpoint()
			: mLine(-1)
			, mEnabled(false)
		{}
	};

	// Represents a character coordinate from the user's point of view,
	// i. e. consider an uniform grid (assuming fixed-width font) on the
	// screen as it is rendered, and each cell has its own coordinate, starting from 0.
	// Tabs are counted as [1..mTabSize] count empty spaces, depending on
	// how many space is necessary to reach the next tab stop.
	// For example, coordinate (1, 5) represents the character 'B' in a line "\tABC", when mTabSize = 4,
	// because it is rendered as "    ABC" on the screen.
	struct Coordinates
	{
		Coordinates() : mLine(0), mColumn(0) {}
		Coordinates(int aLine, int aColumn) : mLine(aLine), mColumn(aColumn)
		{
			assert(aLine >= 0);
			assert(aColumn >= 0);
		}
		static Coordinates Invalid() { static Coordinates invalid(-1, -1); return invalid; }

		bool operator ==(const Coordinates& o) const
		{
			return
				mLine == o.mLine &&
				mColumn == o.mColumn;
		}

		bool operator !=(const Coordinates& o) const
		{
			return
				mLine != o.mLine ||
				mColumn != o.mColumn;
		}

		bool operator <(const Coordinates& o) const
		{
			if (mLine != o.mLine)
				return mLine < o.mLine;
			return mColumn < o.mColumn;
		}

		bool operator >(const Coordinates& o) const
		{
			if (mLine != o.mLine)
				return mLine > o.mLine;
			return mColumn > o.mColumn;
		}

		bool operator <=(const Coordinates& o) const
		{
			if (mLine != o.mLine)
				return mLine < o.mLine;
			return mColumn <= o.mColumn;
		}

		bool operator >=(const Coordinates& o) const
		{
			if (mLine != o.mLine)
				return mLine > o.mLine;
			return mColumn >= o.mColumn;
		}

        int mLine, mColumn;
	};

	struct Identifier
	{
		Coordinates mLocation;
		std::string mDeclaration;
	};

	typedef std::string String;
	typedef std::unordered_map<std::string, Identifier> Identifiers;
	typedef std::unordered_set<std::string> Keywords;
	typedef std::map<int, std::string> ErrorMarkers;
	typedef std::unordered_set<int> Breakpoints;
    typedef std::map<int, std::unordered_set<int>> MmlHighlights;
	typedef std::array<ImU32, (unsigned)PaletteIndex::Max> Palette;
	typedef uint8_t Char;

	struct Glyph
	{
		PaletteIndex mColorIndex = PaletteIndex::Default;
        Char mChar;
		bool mComment : 1;
		bool mMultiLineComment : 1;
		bool mPreprocessor : 1;

		Glyph(Char aChar, PaletteIndex aColorIndex) : mChar(aChar), mColorIndex(aColorIndex),
			mComment(false), mMultiLineComment(false), mPreprocessor(false) {}
	};

	typedef std::vector<Glyph> Line;
	typedef std::vector<Line> Lines;

	struct LanguageDefinition
	{
		typedef std::pair<std::string, PaletteIndex> TokenRegexString;
		typedef std::vector<TokenRegexString> TokenRegexStrings;
        typedef bool(*TokenizeCallback)(const char* in_begin, const char* in_end, const char*& out_begin, const char*& out_end, PaletteIndex& paletteIndex);

		LanguageDefinition()
			: mPreprocChar('#'), mAutoIndentation(true), mTokenize(nullptr), mCaseSensitive(true)
		{
		}

		static const LanguageDefinition& CPlusPlus();
		static const LanguageDefinition& HLSL();
		static const LanguageDefinition& GLSL();
        static const LanguageDefinition& JAVAProcessing();
        static const LanguageDefinition& Python();
        static const LanguageDefinition& Bash();
		static const LanguageDefinition& C();
		static const LanguageDefinition& SQL();
		static const LanguageDefinition& AngelScript();
		static const LanguageDefinition& Lua();
        static const LanguageDefinition& SimpleConsole();

        bool mCaseSensitive;
        bool mAutoIndentation;
        char mPreprocChar;
        TokenizeCallback mTokenize;
        Keywords mKeywords;
        TokenRegexStrings mTokenRegexStrings;
        Identifiers mIdentifiers;
        Identifiers mPreprocIdentifiers;
        std::string mName;
        std::string mCommentStart, mCommentEnd, mSingleLineComment;


    private:
        static void m_HLSLDocumentation(Identifiers& idents);
        static void m_GLSLDocumentation(Identifiers& idents);

	};

	TextEditor();
	~TextEditor();

	void SetLanguageDefinition(const LanguageDefinition& aLanguageDef);
	const LanguageDefinition& GetLanguageDefinition() const { return mLanguageDefinition; }

	const Palette& GetPalette() const { return mPaletteBase; }
	void SetPalette(const Palette& aValue);

	void SetErrorMarkers(const ErrorMarkers& aMarkers) { mErrorMarkers = aMarkers; }
	void SetBreakpoints(const Breakpoints& aMarkers) { mBreakpoints = aMarkers; }
    void SetMmlHighlights(const MmlHighlights& aMarkers) { mMmlHighlights = aMarkers; }

	void Render(const char* aTitle, const ImVec2& aSize = ImVec2(), bool aBorder = false);
    void SetText(const std::string& aText, bool scroll2Top = true);
	std::string GetText() const;

	void SetTextLines(const std::vector<std::string>& aLines);
	std::vector<std::string> GetTextLines() const;

	std::string GetSelectedText() const;
	std::string GetCurrentLineText()const;

	int GetTotalLines() const { return (int)mLines.size(); }
	bool IsOverwrite() const { return mOverwrite; }

	void SetReadOnly(bool aValue);
	bool IsReadOnly() const { return mReadOnly; }
	bool IsTextChanged() const { return mTextChanged; }
	bool IsCursorPositionChanged() const { return mCursorPositionChanged; }

	bool IsColorizerEnabled() const { return mColorizerEnabled; }
	void SetColorizerEnable(bool aValue);

	Coordinates GetCursorPosition() const { return GetActualCursorCoordinates(); }
    void SetCursorPosition(const Coordinates& aPosition, int cursorLineOnPage = -1);

    inline void SetHandleMouseInputs(bool aValue) { mHandleMouseInputs = aValue; }
    inline bool IsHandleMouseInputsEnabled() const { return mHandleMouseInputs; }

    inline void SetHandleKeyboardInputs(bool aValue) { mHandleKeyboardInputs = aValue; }
	inline bool IsHandleKeyboardInputsEnabled() const { return mHandleKeyboardInputs; }

    inline void SetContextMenuEnable    (bool aValue){ mHandleMouseInputs    = aValue;}
    inline bool IsContextMenuEnabled() const { return mHandleKeyboardInputs; }

    inline void SetImGuiChildIgnored(bool aValue) { mIgnoreImGuiChild = aValue; }
	inline bool IsImGuiChildIgnored() const { return mIgnoreImGuiChild; }

	inline void SetShowWhitespaces(bool aValue) { mShowWhitespaces = aValue; }
	inline bool IsShowingWhitespaces() const { return mShowWhitespaces; }

    inline void SetShowShortTabGlyphs(bool aValue) { mShowShortTabGlyphs = aValue; }
    inline bool IsShowingShortTabGlyphs() const { return mShowShortTabGlyphs; }

    inline void SetShowLineNumbers(bool aValue) { mShowLineNumbers = aValue; }
    inline bool IsShowingLineNumbers() const { return mShowLineNumbers; }

    inline bool IsWindowFocused() const { return mWindowIsFocused; }

	void SetTabSize(int aValue);
	inline int GetTabSize() const { return mTabSize; }

	void InsertText(const std::string& aValue);
	void InsertText(const char* aValue);

	void MoveUp(int aAmount = 1, bool aSelect = false);
	void MoveDown(int aAmount = 1, bool aSelect = false);
	void MoveLeft(int aAmount = 1, bool aSelect = false, bool aWordMode = false);
	void MoveRight(int aAmount = 1, bool aSelect = false, bool aWordMode = false);
	void MoveTop(bool aSelect = false);
	void MoveBottom(bool aSelect = false);
	void MoveHome(bool aSelect = false);
	void MoveEnd(bool aSelect = false);

	void SetSelectionStart(const Coordinates& aPosition);
	void SetSelectionEnd(const Coordinates& aPosition);
	void SetSelection(const Coordinates& aStart, const Coordinates& aEnd, SelectionMode aMode = SelectionMode::Normal);
	void SelectWordUnderCursor();
	void SelectAll();
	bool HasSelection() const;

	void Copy();
	void Cut();
	void Paste();
	void Delete();

	bool CanUndo() const;
	bool CanRedo() const;
	void Undo(int aSteps = 1);
	void Redo(int aSteps = 1);

    void Find();

    static const Palette& GetMosaicPalette();
	static const Palette& GetDarkPalette();
	static const Palette& GetLightPalette();
	static const Palette& GetRetroBluePalette();
    static const Palette& GetConsolePalette();

private:
	typedef std::vector<std::pair<std::regex, PaletteIndex>> RegexList;

	struct EditorState
	{
		Coordinates mSelectionStart;
		Coordinates mSelectionEnd;
		Coordinates mCursorPosition;
	};

	class UndoRecord
	{
	public:
		UndoRecord() {}
		~UndoRecord() {}

		UndoRecord(
			const std::string& aAdded,
			const TextEditor::Coordinates aAddedStart,
			const TextEditor::Coordinates aAddedEnd,

			const std::string& aRemoved,
			const TextEditor::Coordinates aRemovedStart,
			const TextEditor::Coordinates aRemovedEnd,

			TextEditor::EditorState& aBefore,
			TextEditor::EditorState& aAfter);

		void Undo(TextEditor* aEditor);
		void Redo(TextEditor* aEditor);

		Coordinates mAddedStart;
		Coordinates mAddedEnd;

		Coordinates mRemovedStart;
		Coordinates mRemovedEnd;

		EditorState mBefore;
		EditorState mAfter;

        std::string mAdded;
        std::string mRemoved;
	};

	typedef std::vector<UndoRecord> UndoBuffer;

	void ProcessInputs();
	void Colorize(int aFromLine = 0, int aCount = -1);
	void ColorizeRange(int aFromLine = 0, int aToLine = 0);
	void ColorizeInternal();
	float TextDistanceToLineStart(const Coordinates& aFrom) const;
    void EnsureCursorVisible(int cursorLineOnPage = -1);
	int GetPageSize() const;
	std::string GetText(const Coordinates& aStart, const Coordinates& aEnd) const;
	Coordinates GetActualCursorCoordinates() const;
	Coordinates SanitizeCoordinates(const Coordinates& aValue) const;
	void Advance(Coordinates& aCoordinates) const;
	void DeleteRange(const Coordinates& aStart, const Coordinates& aEnd);
	int InsertTextAt(Coordinates& aWhere, const char* aValue);
	void AddUndo(UndoRecord& aValue);
    Coordinates ScreenPosToCoordinates(const ImVec2& aPosition, bool aInsertionMode = false) const;
	Coordinates FindWordStart(const Coordinates& aFrom) const;
	Coordinates FindWordEnd(const Coordinates& aFrom) const;
	Coordinates FindNextWord(const Coordinates& aFrom) const;
	int GetCharacterIndex(const Coordinates& aCoordinates) const;
	int GetCharacterColumn(int aLine, int aIndex) const;
	int GetLineCharacterCount(int aLine) const;
	int GetLineMaxColumn(int aLine) const;
	bool IsOnWordBoundary(const Coordinates& aAt) const;
	void RemoveLine(int aStart, int aEnd);
	void RemoveLine(int aIndex);
	Line& InsertLine(int aIndex);
	void EnterCharacter(ImWchar aChar, bool aShift);
	void Backspace();
	void DeleteSelection();
	std::string GetWordUnderCursor() const;
	std::string GetWordAt(const Coordinates& aCoords) const;
	ImU32 GetGlyphColor(const Glyph& aGlyph) const;

	void HandleKeyboardInputs();
	void HandleMouseInputs();
    void HandleContextMenu();
    void RenderInternal(const char* aTitle);

    float mUIScale, mUIFontSize, mEditorFontSize;
    inline float mUICalculateSize(float h)
    {
        return h * (mUIScale + mUIFontSize / 18.0f - 1.0f);
    }
    inline float mEditorCalculateSize(float h)
    {
        return h * (mUIScale + mEditorFontSize / 18.0f - 1.0f);
    }


private:

    bool mHasSearch;

    bool mFindOpened;
    bool mFindJustOpened;
    bool mFindNext;
    bool mFindFocused, mReplaceFocused;

	bool mOverwrite;
	bool mReadOnly;
	bool mWithinRender;
	bool mScrollToCursor;

	bool mScrollToTop;
	bool mTextChanged;
	bool mColorizerEnabled;
	bool mHandleKeyboardInputs;
	bool mHandleMouseInputs;
    bool mContextMenuEnabled;
	bool mIgnoreImGuiChild;
	bool mShowWhitespaces;
    bool mShowShortTabGlyphs;
    bool mShowLineNumbers;
    bool mWindowIsFocused;
    bool mCursorPositionChanged;
	bool mCheckComments;

    int mReplaceIndex;
    int mScrollToCursor_CursorLineOnPage;

    int mLeftMargin;
    int mColorRangeMin, mColorRangeMax;
    int mUndoIndex;
    int mTabSize;
    SelectionMode mSelectionMode;

    char mFindWord[256];
    char mReplaceWord[256];

    float mTextStart;                   // position (in pixels) where a code line starts relative to the left of the TextEditor.
    float mLineSpacing;
    float mLastClick;
    uint64_t mStartTime;
    ImVec2 mCharAdvance;
    MmlHighlights mMmlHighlights;
	Coordinates mInteractiveStart, mInteractiveEnd;
    EditorState mState;
    ErrorMarkers mErrorMarkers;
    Lines mLines;
    RegexList mRegexList;
    UndoBuffer mUndoBuffer;
	std::string mLineBuffer;
    Breakpoints mBreakpoints;
    Palette mPaletteBase;
    Palette mPalette;
    LanguageDefinition mLanguageDefinition;
};
