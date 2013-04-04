// Copyright (C) 2007-2013  CEA/DEN, EDF R&D
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
// Author : Adrien Bruneton (CEA/DEN)
// Created on: 4 avr. 2013

#include "PyConsole.h"
#include <Python.h>

#include <QKeyEvent>
#include <QTextBlock>
#include <QTextCursor>
#include <QTextCharFormat>
#include <QRegExp>

#include "PyConsole_EnhEditor.h"
#include "PyConsole_EnhInterp.h"
#include "PyConsole_Request.h"
#include "PyInterp_Dispatcher.h"

// Initialize list of valid separators
static const char * tmp_a[] = {" ", "(", "[","+", "-", "*", "/", ";", "^", "="};
const std::vector<QString> PyConsole_EnhEditor::SEPARATORS = \
    std::vector<QString>(tmp_a, tmp_a + sizeof(tmp_a)/sizeof(tmp_a[0]));

PyConsole_EnhEditor::PyConsole_EnhEditor(PyConsole_EnhInterp * interp, QWidget * parent) :
     PyConsole_Editor(interp, parent),
     _tab_mode(false),
     _cursor_pos(-1)
{
  document()->setUndoRedoEnabled(true);
}

void PyConsole_EnhEditor::keyPressEvent ( QKeyEvent* event)
{
  // check if <Ctrl> is pressed
  bool ctrlPressed = event->modifiers() & Qt::ControlModifier;
  // check if <Shift> is pressed
  bool shftPressed = event->modifiers() & Qt::ShiftModifier;

  if (event->key() == Qt::Key_Tab && !shftPressed)
    {
      if (!ctrlPressed)
        handleTab();
      else
        {
          clearCompletion();
          handleBackTab();
        }
      PyConsole_Editor::keyPressEvent(event);
    }
  else
    {
      if (!ctrlPressed)
        {
          clearCompletion();
          _cursor_pos = -1;
        }
      PyConsole_Editor::keyPressEvent(event);
    }
}

void PyConsole_EnhEditor::mousePressEvent(QMouseEvent* e)
{
  clearCompletion();
  _cursor_pos = -1;
  PyConsole_Editor::mousePressEvent(e);
}

void PyConsole_EnhEditor::clearCompletion()
{
  // Delete completion text if present
  if (_tab_mode)
    {
      // Remove completion display
      document()->undo();
      // Remove trailing line return:
      QTextCursor tc(textCursor());
      tc.setPosition(document()->characterCount()-1);
      setTextCursor(tc);
      textCursor().deletePreviousChar();
      // TODO: before wait for any TAB event to be completed
      static_cast<PyConsole_EnhInterp *>(myInterp)->clearCompletion();
    }
  _tab_mode = false;
}

void PyConsole_EnhEditor::handleTab()
{
  if (_tab_mode)
    {
      // Already tab mode - nothing to do !
      return;
    }

  QTextCursor cursor(textCursor());

  // Cursor at end of input
  cursor.movePosition(QTextCursor::End);
  setTextCursor(cursor);

  // Save cursor position if needed
  if (_cursor_pos == -1)
    _cursor_pos = textCursor().position();

  // get last line
  QTextBlock par = document()->end().previous();
  if ( !par.isValid() ) return;

  // Switch to completion mode
  _tab_mode = true;

  QString cmd = par.text().mid(promptSize());

  // Post completion request
  // Editor will be informed via a custom event that completion has been run
  PyInterp_Request* req = createTabRequest(cmd);
  PyInterp_Dispatcher::Get()->Exec(req);
}

void PyConsole_EnhEditor::handleBackTab()
{
  QTextCursor cursor(textCursor());

  if (_cursor_pos == -1)
    {
      // Invalid cursor position - we can't do anything:
      return;
    }
  // Ensure cursor is at the end of command line
  cursor.setPosition(_cursor_pos);
  cursor.movePosition(QTextCursor::EndOfLine);
  //setCursor(cursor);

  // Delete last completed text
  int i = cursor.position() - _cursor_pos;
  cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor, i);
  cursor.removeSelectedText();
  _cursor_pos = -1;
}

PyInterp_Request* PyConsole_EnhEditor::createTabRequest( const QString& input )
{
  // Parse input to extract on what part the dir() has to be executed
  QString input2(input);

  // Split up to the last syntaxical separator
  int lastSp = -1;
  for (std::vector<QString>::const_iterator i = SEPARATORS.begin(); i != SEPARATORS.end(); i++)
    {
      int j = input2.lastIndexOf(*i);
      if (j > lastSp)
        lastSp = j;
    }
  if (lastSp >= 0)
    input2 = input.mid(lastSp+1);

  // Detect a qualified name (with a point)
  int lastPt = input2.lastIndexOf(QString("."));

  // Split the 2 surrounding parts of the qualified name
  if (lastPt != -1)
    {
      _compl_before_point = input2.left(lastPt);
      _compl_after_point = input2.mid(lastPt+1);
    }
  else
    {
      // No point found - do a global matching -
      // (the following will call dir() with an empty string)
      _compl_after_point = input2;
      _compl_before_point = QString("");
    }

  return new CompletionCommand( static_cast<PyConsole_EnhInterp *>(myInterp), _compl_before_point,
                               _compl_after_point, this, isSync() );
}

/**
 * Format completion results - this is where we should create 3 columns etc ...
 * @param matches list of possible completions
 * @param result return value
 */
void PyConsole_EnhEditor::formatCompletion(const std::vector<QString> & matches, QString & result) const
{
  int sz = matches.size();

  if (sz > MAX_COMPLETIONS)
    {
      sz = MAX_COMPLETIONS;
      result.append("[Too many matches! Displaying first ones only ...]\n");
    }

  for (int i = 0; i < sz; ++i)
    {
      result.append(matches[i]);
      result.append("\n");
    }
}

void PyConsole_EnhEditor::customEvent( QEvent* event )
{
  std::vector<QString> matches;
  QString first_match, comple_text, doc, base;
  QTextCursor cursor(textCursor());
  QTextBlockFormat bf;
  QTextCharFormat cf;
  PyConsole_EnhInterp * interp = static_cast<PyConsole_EnhInterp *>(myInterp);

  switch( event->type() )
  {
    case PyInterp_Event::ES_TAB_COMPLETE_OK:
      // Extract corresponding matches from the interpreter
      matches = interp->getLastMatches();

      if (matches.size() == 0)
        {
          // Completion successful but nothing returned.
          _tab_mode = false;
          _cursor_pos = -1;
          return;
        }

      // Only one match - complete directly and update doc string window
      doc = interp->getDocStr();
      if (matches.size() == 1)
        {
          first_match = matches[0].mid(_compl_after_point.size());
          cursor.insertText(first_match);
          _tab_mode = false;
          if (doc == QString(""))
            emit updateDoc(formatDocHTML("(no documentation available)\n"));
          else
            emit updateDoc(formatDocHTML(doc));
        }
      else
        {
          // Detect if there is a common base to all available completion
          // In this case append this base to the text already
          extractCommon(matches, base);
          first_match = base.mid(_compl_after_point.size());
          cursor.insertText(first_match);

          // If this happens to match exaclty the first completion
          // also provide doc
          if (base == matches[0])
            {
              doc = formatDocHTML(doc);
              emit updateDoc(doc);
            }

          // Print all matching completion in a "undo-able" block
          cursor.insertBlock();
          cursor.beginEditBlock();

          // Insert all matches
          QTextCharFormat cf;
          cf.setForeground(QBrush(Qt::darkGreen));
          cursor.setCharFormat(cf);
          formatCompletion(matches, comple_text);
          cursor.insertText(comple_text);
          cursor.endEditBlock();
        }
      break;
    case PyInterp_Event::ES_TAB_COMPLETE_ERR:
      // Tab completion was unsuccessful, switch off mode:
      _tab_mode = false;
      _cursor_pos = -1;
      break;
//    case PyEnhInterp_Event::ES_MULTI_PASTE:
//      multilinePaste();
//      break;
    default:
      PyConsole_Editor::customEvent( event );
      break;
  }
}

/**
 * Extract the common leading part of all strings in matches.
 * @param matches
 * @param result
 */
void PyConsole_EnhEditor::extractCommon(const std::vector<QString> & matches, QString & result) const
{
  result = "";
  int charIdx = 0;

  if (matches.size() < 2)
    return;

  while (true)
    {
      if (charIdx >= matches[0].size())
        return;
      QChar ch = matches[0][charIdx];
      for (int j = 1; j < matches.size(); j++)
        if (charIdx >= matches[j].size() || matches[j][charIdx] != ch)
          return;
      result += ch;
      charIdx++;
    }
}

QString PyConsole_EnhEditor::formatDocHTML(const QString & doc) const
{
  QString templ = QString("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" ") +
      QString(" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n ") +
      QString("<html><head><meta name=\"qrichtext\" content=\"1\" /> ") +
      QString("<style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style> ") +
      QString("</head><body style=\" font-family:'Sans Serif'; font-size:10pt; font-weight:400; font-style:normal;\">\n") +
      QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">") +
      QString("<span style=\" font-weight:600; color:#0000ff;\">%1</span></p>") +
      QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">%2</p>") +
      QString("</body></html>");

  QString fst, rest("");

  // Extract first line of doc
  int idx = doc.indexOf("\n");
  if (idx > 0)
    {
      fst = doc.left(idx);
      rest = doc.mid(idx+1);
    }
  else
    {
      fst = doc;
    }

  fst = fst.replace("\n", " ");
  rest = rest.replace("\n", " ");
  return templ.arg(fst).arg(rest);
}
