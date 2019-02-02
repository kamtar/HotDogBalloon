/*
 * ICrypto.hpp
 *
 *  Created on: 2 Feb 2019
 *      Author: Kamtar
 */

#ifndef UTILS_ICRYPTOCONTEXT_HPP_
#define UTILS_ICRYPTOCONTEXT_HPP_
#include <DataRef.hpp>

class ICryptoContext
{
public:
	virtual void Init() = 0;
	virtual void SetKey(SimpleDataRef key) = 0;
	virtual void Ecrypt(SimpleDataRef data_in, SimpleDataRef data_out) = 0;
	virtual void Decrypt(SimpleDataRef data_in, SimpleDataRef data_out) = 0;
};



#endif /* UTILS_ICRYPTOCONTEXT_HPP_ */
