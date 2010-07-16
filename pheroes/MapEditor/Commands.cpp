/*
 * This file is a part of Pocket Heroes Game project
 * 	http://www.pocketheroes.net
 *	https://code.google.com/p/pocketheroes/
 *
 * Copyright 2004-2010 by Robert Tarasov and Anton Stuk (iO UPG)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */ 

#include "stdafx.h"
#include "MinimapView.h"
#include "MainView.h"
#include "Commands.h"

const TCHAR*
Command::Name() const
{ 
	return _T("Unknown");
}

CompositeCommand::CompositeCommand( const TCHAR* name )
{
	if ( name ) name_ = _T("Composite");
	else name_ = name;
}

CompositeCommand::~CompositeCommand()
{
	cmds_.DeleteAll();
}

void 
CompositeCommand::Commit( iMapHandler* map )
{
	for( CommandList::Iterator it = cmds_.First(); it != cmds_.End(); ++it )
		it->Commit( map );
}

void
CompositeCommand::Rollback( iMapHandler* map )
{
	for( CommandList::Iterator it = cmds_.Last(); it != cmds_.End(); --it )
		it->Rollback( map );
}

void
CompositeCommand::Push( Command* cmd )
{
	cmds_.Add( cmd );
}

Command*
CompositeCommand::Pop()
{
	return cmds_.Remove();
}

const TCHAR*
CompositeCommand::Name() const
{
	return name_.CStr();
}

//

CommandManager::CommandManager()
{
}

CommandManager::~CommandManager()
{
	undoList_.DeleteAll();
	redoList_.DeleteAll();
}

bool
CommandManager::CanUndo() const
{
	return 0 != undoList_.Count();
}

bool
CommandManager::CanRedo() const
{
	return 0 != redoList_.Count();
}

void
CommandManager::ClearRedo()
{
	redoList_.DeleteAll();
}

void
CommandManager::ClearAll() 
{
	undoList_.DeleteAll();
	redoList_.DeleteAll();
}

void
CommandManager::CommitCommand( Command* cmd, iMapHandler* map )
{
	if ( !group_ ) {
		// Cleanup redo 
		ClearRedo();

		// execute
		cmd->Commit( map );

		// add to undo list
		undoList_.Add( cmd );
	
		LimitStacks();
	} else {
		// execute
		cmd->Commit( map );

		// add to the group
		group_->Push( cmd );
	}
}

bool
CommandManager::Undo(iMapHandler* map)
{
	if ( 0 == undoList_.Count() ) return false;

	Command* cmd = undoList_.Remove();
	ATLASSERT( cmd != 0 );

	// execute
	cmd->Rollback( map );

	// move to redo
	redoList_.Add( cmd );

	LimitStacks();
	return true;
}

bool
CommandManager::Redo(iMapHandler* map)
{
	if ( 0 == redoList_.Count() ) return false;
	
	Command* cmd = redoList_.Remove();
	ATLASSERT( cmd != 0 );

	// execute
	cmd->Commit( map );

	// move to undo
	undoList_.Add( cmd );

	LimitStacks();
	return true;
}

void
CommandManager::StartGroup( const TCHAR* name )
{
	ATLASSERT( name != 0 );
	FlushGroup();
	group_.Reset( new CompositeCommand( name ) );
}

void
CommandManager::EndGroup()
{
	FlushGroup();
}

void
CommandManager::FlushGroup()
{
	if ( !group_ ) return;

	ClearRedo();
	undoList_.Add( group_.Giveup() );
	LimitStacks();
}

void
CommandManager::LimitStacks()
{
	const size_t MaxDepth = 32;

	while ( undoList_.Count() > MaxDepth )
		undoList_.Delete( 0 );

	while ( redoList_.Count() > MaxDepth )
		redoList_.Delete( 0 );
}

//

DrawTileAtLocation::DrawTileAtLocation( iPoint pt, SURF_TYPE surf )
{

}

const TCHAR*
DrawTileAtLocation::Name() const
{
	return _T("Draw Tile");
}

void
DrawTileAtLocation::Commit( iMapHandler* map )
{
}

void
DrawTileAtLocation::Rollback( iMapHandler* map )
{
}

//

AddObjectCommand::AddObjectCommand(iBaseNode* obj, iPoint pos, PLAYER_ID pid )
: obj_(obj)
, pos_(pos)
, pid_(pid)
{}

void
AddObjectCommand::Commit(iMapHandler* map )
{
	check( map );
	// erase & take type of the object underneath

	map->AddObject( obj_, pid_, pos_ );
}

void
AddObjectCommand::Rollback(iMapHandler* map )
{
	check( map );
}

const TCHAR*
AddObjectCommand::Name() const
{
	return _T("Add object");
}