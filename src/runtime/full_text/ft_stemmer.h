/*
 * Copyright 2006-2008 The FLWOR Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
#ifndef ZORBA_RUNTIME_FULL_TEXT_FT_STEMMER_H
#define ZORBA_RUNTIME_FULL_TEXT_FT_STEMMER_H

#include <string>

#include "runtime/full_text/stemmer/include/libstemmer.h"
#include "zorbautils/lang.h"
#include "zorbautils/mutex.h"

namespace zorba {

class ft_stemmer {
public:
  ~ft_stemmer();

  static ft_stemmer const* get( lang::iso639_1::type );

  void stem( std::string const &word, std::string &result ) const;

private:
  sb_stemmer *const stemmer_;
  mutable Mutex mutex_;

  ft_stemmer( lang::iso639_1::type );

  // forbid these
  ft_stemmer( ft_stemmer const& );
  ft_stemmer& operator=( ft_stemmer const& );
};

} // namespace zorba
#endif  /* ZORBA_RUNTIME_FULL_TEXT_FT_STEMMER_H */
/* vim:set et sw=2 ts=2: */