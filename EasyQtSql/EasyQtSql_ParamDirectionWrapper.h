#ifndef EASYQTSQL_PARAMETERWRAPPER_H
#define EASYQTSQL_PARAMETERWRAPPER_H

/*
 * The MIT License (MIT)
 * Copyright 2018 Alexey Kramin
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
*/

#ifndef EASY_QT_SQL_MAIN

#include <QtCore>

#endif

/*!
 * \brief Base parameters wrapper struct
 *
 * The ParamDirectionWrapper struct is pure-virtual
 */
struct ParamDirectionWrapper
{
   ParamDirectionWrapper(const QVariant &val, const QString &aliasName)
    : value(val)
    , alias(aliasName.trimmed())
   { }

   virtual ~ParamDirectionWrapper() {}

   const QVariant value;
   const QString  alias;

   /*!
    * \brief Protection against instantiating the class
    */
   virtual void doesNothing() const = 0;
};

/*!
 * \brief Input SQL parameters wrapper
 */
struct  In : public ParamDirectionWrapper
{
   In(const QVariant &value, const QString &alias = QString())
    : ParamDirectionWrapper(value, alias.trimmed().toLower())
   { }

   /*!
    * \brief Does nothing
    */
   virtual void doesNothing() const override {}
};

/*!
 * \brief Output SQL parameters wrapper
 */
struct  Out : public ParamDirectionWrapper
{
   explicit Out(const QString &alias)
    : ParamDirectionWrapper(QVariant(), alias)
   { }

   /*!
    * \brief Does nothing
    */
   virtual void doesNothing() const override {}
};

/*!
 * \brief Bidirectional SQL parameters wrapper
 */
struct  InOut : public ParamDirectionWrapper
{
   InOut(const QVariant &value, const QString &alias)
    : ParamDirectionWrapper(value, alias)
   { }

   /*!
    * \brief Does nothing
    */
   virtual void doesNothing() const override {}
};

#endif // EASYQTSQL_PARAMETERWRAPPER_H
