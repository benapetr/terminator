 // This program is free software; you can redistribute it and/or modify
 // it under the terms of the GNU General Public License as published by
 // the Free Software Foundation; either version 2 of the License, or
 // (at your option) version 3.

 // This program is distributed in the hope that it will be useful,
 // but WITHOUT ANY WARRANTY; without even the implied warranty of
 // MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 // GNU General Public License for more details.

#ifndef ITEM_H
#define ITEM_H

#include <iostream>

using namespace std;

namespace terminator
{
    class Item
    {
        public:
            Item(string file, string text);
            virtual ~Item();
            string File;
            string Text;
        protected:
        private:
    };
}

#endif // ITEM_H
