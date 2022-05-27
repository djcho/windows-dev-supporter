#pragma once

#define		RET_INVALID_NULL_PARAM(logger, x)						\
			if(!x)													\
			{														\
				LOG4CPLUS_TRACE_END(logger);						\
				return RETURN_REG_CONF_INVALID_NULL_PARAM;			\
			}

#define		RET_INVALID_STRING_EMPTY_PARAM(logger, x)				\
			if(x.empty())											\
			{														\
				LOG4CPLUS_TRACE_END(logger);						\
				return RETURN_REG_CONF_INVALID_STRING_EMPTY_PARAM;	\
			}

#define		RET_INVALID_BUF_SIZE(logger, internalsize, inputsize)	\
			if(internalsize < inputsize)							\
			{														\
				LOG4CPLUS_TRACE_END(logger);						\
				return RETURN_REG_CONF_INVALID_BUF_SIZE;			\
			}
