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

//
//

#ifndef HMM_EDITOR_COMMAND_H_
#define HMM_EDITOR_COMMAND_H_

#include "ScopePtr.h"

class iMapHandler;

class Command : public iIListNode
{
public:
	virtual ~Command() {}

	virtual void Commit( iMapHandler* map )  =0;
	virtual void Rollback( iMapHandler* map ) =0;

	virtual const TCHAR* Name() const;
};

typedef iIList<Command> CommandList;
typedef ScopedPtr<Command> CommandPtr;

//

class CompositeCommand : public Command
{
public:
	CompositeCommand( const TCHAR* name = 0 );
	~CompositeCommand();

	virtual void Commit( iMapHandler* map );
	virtual void Rollback( iMapHandler* map );

	void Push( Command* );
	Command* Pop();

	virtual const TCHAR* Name() const;

private:
	CommandList	cmds_;
	iStringT	name_;
};

typedef ScopedPtr<CompositeCommand> CompositeCommandPtr;


//

class AddObjectCommand : public Command
{
public:
	AddObjectCommand( iBaseNode* obj, iPoint pos, PLAYER_ID pid = PID_NEUTRAL );
	virtual void Commit( iMapHandler* map );
	virtual void Rollback( iMapHandler* map );
	virtual const TCHAR* Name() const;
private:
	iBaseNode*	obj_;
	iPoint		pos_;
	PLAYER_ID	pid_;
};

class EraseAllAtLocation : public Command
{
public:
	virtual void Commit( iMapHandler* map );
	virtual void Rollback( iMapHandler* map );
	virtual const TCHAR* Name() const;
};

class CreateEntityAtLocation : public Command
{
};

class DrawTileAtLocation : public Command
{
public:
	DrawTileAtLocation( iPoint pt, SURF_TYPE surf );

	virtual void Commit( iMapHandler* map );
	virtual void Rollback( iMapHandler* map );
	virtual const TCHAR* Name() const;

private:
	iPoint		pos_;
	SURF_TYPE	new_;
	SURF_TYPE	old_;
};

//

class CommandManager
{
public:

	CommandManager();
	~CommandManager();

	bool CanUndo() const;
	bool CanRedo() const;

	void ClearRedo();
	void ClearAll();

	void CommitCommand( Command* cmd, iMapHandler* );
	bool Undo( iMapHandler* );
	bool Redo( iMapHandler* );

	void StartGroup( const TCHAR* name );
	void EndGroup();
	
private:

	void LimitStacks();
	void FlushGroup();

	CompositeCommandPtr	group_;
	CommandList			undoList_;
	CommandList			redoList_;
};

#endif //#define HMM_EDITOR_COMMAND_H_

