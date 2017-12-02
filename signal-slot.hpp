#ifndef __SIGNAL__SLOT__
#define __SIGNAL__SLOT__

#include <functional>
#include <list>
#include <memory>

template<class SignalType>
class ConnectionBase{
	public:
		ConnectionBase(SignalType *signal)
			:m_signal(signal){}
		~ConnectionBase(){}
		
		//disable copy
		ConnectionBase(const ConnectionBase&) = delete;
		ConnectionBase &operator=(ConnectionBase&) = delete;
		
		void m_disconnect(){
			if(m_is_vaild()){
				m_signal->m_remove_connection(this);
				m_set_invaild();
			}
		}
		
		bool m_is_vaild(){
			return (m_signal != nullptr);
		}
		
		void m_set_invaild(){
			m_signal = nullptr;
		}
	private:
		SignalType *m_signal;
};

template<class ConnectionType>
class Connection{
	public:
		Connection(std::shared_ptr<ConnectionType> connection)
			:m_raw_connection(connection){}
		Connection(const Connection &connection){
			m_raw_connection = connection.m_raw_connection;
		}
		Connection &operator=(const Connection &connection){
			m_raw_connection = connection.m_raw_connection;
		}
		~Connection(){
			m_raw_connection.reset();
		}
	
		void disconnect(){
			m_raw_connection->m_disconnect();
		}
		bool isVaild(){
			return m_raw_connection->m_is_vaild();
		}
	private:
		std::shared_ptr<ConnectionType> m_raw_connection;
};

template<class Type>
class Signal{};

template<class ReturnType,class...ArgsType>
class Signal<ReturnType(ArgsType...)>{
	public:
		Signal(){}
		~Signal(){
			for(auto &itr:m_pairs){
				itr.first->m_set_invaild();
				itr.first.reset();
				delete itr.second;
			}
		}
		
		void m_remove_connection(ConnectionBase<
									Signal<ReturnType(ArgsType...)>
								> *connection){
			auto itr = m_pairs.begin();
			auto end = m_pairs.end();
			for(;itr!=end;itr++){
				if(itr->first.get() == connection){
					itr->first.reset();
					delete itr->second;
					m_pairs.erase(itr);
					return;
				}
			}
		}
		
		template<class Function>
		Connection<
			ConnectionBase<
				Signal<ReturnType(ArgsType...)>
			>
		> connect(Function &&f){
			m_pairs.push_back(
				std::make_pair<
					std::shared_ptr<
						ConnectionBase<
							Signal<ReturnType(ArgsType...)>
						>
					>,
					std::function<ReturnType(ArgsType...)> *
				>(std::make_shared<ConnectionBase<Signal<ReturnType(ArgsType...)>>>(this),
				  new std::function<ReturnType(ArgsType...)>(std::forward<Function>(f)))
			);
			return Connection<
					ConnectionBase<
						Signal<ReturnType(ArgsType...)>
					>>(m_pairs.rbegin()->first);
		}
		
		template<class...ArgsType2>
		typename std::enable_if<
			!std::is_void<ReturnType>::value,
			ReturnType
		>::type emit(ArgsType2&&...args){
			ReturnType ret;
			for(auto &itr:m_pairs){
				ret = (*(itr.second))(std::forward<ArgsType>(args)...);
			}
			return ret;
		}
	private:
		std::list<
			std::pair<
				std::shared_ptr<
					ConnectionBase<
						Signal<ReturnType(ArgsType...)>
					>
				>,
				std::function<ReturnType(ArgsType...)> *
			>
		> m_pairs;
};

template<class...ArgsType>
class Signal<void(ArgsType...)>{
	public:
		Signal(){}
		~Signal(){
			for(auto &itr:m_pairs){
				itr.first->m_set_invaild();
				itr.first.reset();
				delete itr.second;
			}
		}
		
		void m_remove_connection(ConnectionBase<
									Signal<void(ArgsType...)>
								> *connection){
			auto itr = m_pairs.begin();
			auto end = m_pairs.end();
			for(;itr!=end;itr++){
				if(itr->first.get() == connection){
					itr->first.reset();
					delete itr->second;
					m_pairs.erase(itr);
					return;
				}
			}
		}
		
		template<class Function>
		Connection<
			ConnectionBase<
				Signal<void(ArgsType...)>
			>
		> connect(Function &&f){
			m_pairs.push_back(
				std::make_pair<
					std::shared_ptr<
						ConnectionBase<
							Signal<void(ArgsType...)>
						>
					>,
					std::function<void(ArgsType...)> *
				>(std::make_shared<ConnectionBase<Signal<void(ArgsType...)>>>(this),
				  new std::function<void(ArgsType...)>(std::forward<Function>(f)))
			);
			return Connection<
					ConnectionBase<
						Signal<void(ArgsType...)>
					>>(m_pairs.rbegin()->first);
		}
		
		template<class...ArgsType2>
		void emit(ArgsType2&&...args){
			for(auto &itr:m_pairs){
				(*(itr.second))(std::forward<ArgsType>(args)...);
			}
		}
	private:
		std::list<
			std::pair<
				std::shared_ptr<
					ConnectionBase<
						Signal<void(ArgsType...)>
					>
				>,
				std::function<void(ArgsType...)> *
			>
		> m_pairs;
};

#endif
