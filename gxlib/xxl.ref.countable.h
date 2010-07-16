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

#ifndef XXL_REF_COUNTABLE_H__
#define XXL_REF_COUNTABLE_H__

//
// base abstract
class ref_countable
{
public:
	virtual 		~ref_countable() 	{}
	virtual void    add_ref() const		=0;
	virtual void	release() const		=0;
};

//
// ref countable wrapper
template< typename T >
class ref_countable_impl : public T
{
public:
	ref_countable_impl()
	: refc_(0)
	{}

	virtual ~ref_countable_impl()
	{}

public:
	virtual void add_ref() const
	{ refc_++; }

	virtual void release() const
	{
		check( refc_ != 0 );
		if ( --refc_ == 0 ) delete this;
	}

private:
	mutable size_t refc_;
};

#endif //XXL_REF_COUNTABLE_H__

