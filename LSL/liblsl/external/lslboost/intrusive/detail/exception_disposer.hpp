/////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga  2014-2014
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.lslboost.org/LICENSE_1_0.txt)
//
// See http://www.lslboost.org/libs/intrusive for documentation.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTRUSIVE_DETAIL_EXCEPTION_DISPOSER_HPP
#define BOOST_INTRUSIVE_DETAIL_EXCEPTION_DISPOSER_HPP

#ifndef BOOST_CONFIG_HPP
#  include <lslboost/config.hpp>
#endif

#if defined(BOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

namespace lslboost {
namespace intrusive {
namespace detail {

template<class Container, class Disposer>
class exception_disposer
{
   Container *cont_;
   Disposer  &disp_;

   exception_disposer(const exception_disposer&);
   exception_disposer &operator=(const exception_disposer&);

   public:
   exception_disposer(Container &cont, Disposer &disp)
      :  cont_(&cont), disp_(disp)
   {}

   void release()
   {  cont_ = 0;  }

   ~exception_disposer()
   {
      if(cont_){
         cont_->clear_and_dispose(disp_);
      }
   }
};

template<class Container, class Disposer, class SizeType>
class exception_array_disposer
{
   Container *cont_;
   Disposer  &disp_;
   SizeType  &constructed_;

   exception_array_disposer(const exception_array_disposer&);
   exception_array_disposer &operator=(const exception_array_disposer&);

   public:

   exception_array_disposer
      (Container &cont, Disposer &disp, SizeType &constructed)
      :  cont_(&cont), disp_(disp), constructed_(constructed)
   {}

   void release()
   {  cont_ = 0;  }

   ~exception_array_disposer()
   {
      SizeType n = constructed_;
      if(cont_){
         while(n--){
            cont_[n].clear_and_dispose(disp_);
         }
      }
   }
};

}  //namespace detail{
}  //namespace intrusive{
}  //namespace lslboost{

#endif //BOOST_INTRUSIVE_DETAIL_EXCEPTION_DISPOSER_HPP