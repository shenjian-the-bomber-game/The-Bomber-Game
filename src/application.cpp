#include "../include/application.hpp"

using namespace fly;
using namespace std;
const string InitPassword = "123456";

extern PresentationLayer PreLayerInstance;
extern TransferLayer TransLayerInstance;

DatabaseConnection *DatabaseConnection::obj = NULL;
ApplicationLayer::ApplicationLayer()
{
        // initialize DatabaseConnection class
        DatabaseConnection::get_instance()->DatabaseInit();

        return;
}

bool ApplicationLayer::CheckUser(std::string user_name_)
{
        return DatabaseConnection::get_instance()->check_account(user_name_);
}

bool ApplicationLayer::CheckPasswd(std::string user_name_, std::string password_)
{
        return DatabaseConnection::get_instance()->check_password(user_name_, password_);
}

bool ApplicationLayer::ResetPasswd(std::string user_name_, std::string password_) 
{
        return DatabaseConnection::get_instance()->reset_password(user_name_, password_);
}

void ApplicationLayer::MessageToApp(Client *client_name_)
{
        // main process here
        // finite state machine
        Message_To_App *message_ = &client_name_->message_ptoa;
        Message_To_Pre *respond_ = &client_name_->message_atop;
        switch(client_name_->state)
        {
                case SessionState::Acceptance: {
                        // if(message_->type_ != PacketType::Info){
                        //         // error occurs
                        //         // client_name_.message_atop.respond_ = ResponseType::ErrorOccurs;
                        //         respond_->respond_ = ResponseType::ErrorOccurs;
                        //         PreLayerInstance.pack_Message(client_name_);
                        //         LOG(Error) << "Error receive info packet: " << unsigned(uint8_t(message_->type_)) << endl 
                        //             << "Username: " << message_->user_name_ << std::endl;
                        //         return;
                        //         // stop the connection 
                        //         // client_name_->state = SessionState::Error;
                        //         // respond_.type_ = PacketType::InfoResponse;
                        // }        
                        // do recv info packet
                        switch(CheckUser(message_->user_name_)){
                               case true: {
                                       // account exists
                                        respond_->type_ = PacketType::InfoResponse;
                                        respond_->respond_ = ResponseType::OK;
                                        client_name_->state = SessionState::WaitForPasswd;
                                        client_name_->host_username_ = message_->user_name_;
                                        LOG(Info) << "Check User Exists" << std::endl;
                                        PreLayerInstance.pack_Message(client_name_);
                                       break;
                               }
                               case false: {
                                       // account not exists
                                        respond_->type_ = PacketType::InfoResponse;
                                        respond_->respond_ = ResponseType::UserNotExist;
                                        client_name_->state = SessionState::Error;
                                        LOG(Error) << "User not Exists" << std::endl;
                                        PreLayerInstance.pack_Message(client_name_);
                                        break;
                               }
                        }
                        break;
                }
                case SessionState::WaitForPasswd: {
                //         if(message_->type_ != PacketType::Password) {
                //                 // error occurs
                //                 LOG(Error) << "Error receive password packet" << std::endl;
                //                 // stop the connection
                //                 client_name_->state = SessionState::Error;
                //                 respond_->type_ = PacketType::PasswordResponse;
                //                 respond_->respond_ = ResponseType::ErrorOccurs;
                //                 PreLayerInstance.pack_Message(client_name_);
                //                 return;
                //         }
                        // do recv password packet
                        switch(CheckPasswd(client_name_->host_username_, message_->password_)) {
                                case true: {
                                        // password correct
                                        Client * client_temp;
                                        if((client_temp = TransLayerInstance.find_by_username_cnt(client_name_)) !=NULL) {
                                                client_temp->message_atop.type_ = PacketType::Refuse;
                                                client_temp->message_atop.respond_ = ResponseType::ErrorOccurs;
                                                PreLayerInstance.pack_Message(client_temp);
                                                respond_->type_ = PacketType::Refuse;
                                                respond_->respond_ = ResponseType::AlreadyLoggedIn;
                                                PreLayerInstance.pack_Message(client_name_);
                                        }
                                        respond_->type_ = PacketType::PasswordResponse;
                                        respond_->respond_ = ResponseType::OK;
                                        PreLayerInstance.pack_Message(client_name_);
                                        client_name_->state = SessionState::ServerWaiting;
                                        respond_->type_ = PacketType::OnlineList;
                                        // TODO FIND ALL THE ONLINE USERS
                                        respond_->onlineuser_ = TransLayerInstance.find_all_user();
                                        // respond_->config_ = DatabaseConnection::get_instance()->retrive_history_count(client_name_->host_username_);
                                        PreLayerInstance.pack_Message(client_name_);
                                        break;
                                }
                                case false: {
                                        // password error
                                        LOG(Info) << "Recv Wrong Password" << endl;
                                        client_name_->state = SessionState::Error;
                                        respond_->type_ = PacketType::PasswordResponse;
                                        respond_->respond_ = ResponseType::WrongPassword;
                                        PreLayerInstance.pack_Message(client_name_);
                                        break;
                                }
                                break;
                        }
                        break;
                }
                case SessionState::ServerWaiting: {
                        switch(message_->type_) {
                                case PacketType::SendInvit: {
                                        LOG(Info) << "Client A send invitation to Cline B" << endl;
                                        // LOG(Debug) << message_->user_name_ << endl;
                                        Client* Client_B;
                                        if((Client_B = TransLayerInstance.find_by_username(message_->user_name_b_)) !=NULL) {
                                                if(client_B->state != SessionState::ServerWaiting) {
                                                        respond_->type_ = PacketType::InvitResponse;
                                                        respond_->respond_ = ResponseType::Busy;
                                                        PreLayerInstance.pack_Message(client_name_);
                                                }
                                                else {
                                                        Client_B->message_atop.user_name_a_ = client_name_->host_username_;
                                                        Client_B->message_atop.type_ = PacketType::RecvInvit;
                                                        PreLayerInstance.pack_Message(Client_B);
                                                }
                                        }
                                        else {
                                                respond_->type_ = PacketType::InvitResponse;
                                                respond_->respond_ = ResponseType::UserNotExist;
                                                PreLayerInstance.pack_Message(client_name_);
                                        }
                                        break;
                                }
                                case PacketType::InvitResponse: {
                                        LOG(Info) << "Client A recv invitation response from CLinet B" << endl;
                                        LOG(Info) << "Server B need to forward the message to CLient A" << endl;
                                        if(message_->respond_ == ResponseType::OK) {
                                                // Clinet A need to know that its invitation works
                                                Client* Client_A;
                                                if((Client_A = TransLayerInstance.find_by_username(respond_->user_name_a_)) != NULL) {
                                                       Client_A->message_atop.type_ = PacketType::InvitResponse;
                                                       Client_A->message_atop.respond_ = ResponseType::OK;
                                                       PreLayerInstance.pack_Message(ClieClient_Bnt_A); 
                                                       Client_A->state = SessionState::WaitForBoard;
                                                       client_name_->state = SessionState::WaitForBoard;
                                                }
                                                else {
                                                        LOG(Error) << "Can't Find Client A after B responsed." << endl;
                                                }
                                        }
                                        else if(message_->respond_ == ResponseType::Refuse) {
                                                Client* Client_A;
                                                if((Client_A = TransLayerInstance.find_by_username(respond_->user_name_a_)) != NULL) {
                                                       Client_A->message_atop.type_ = PacketType::InvitResponse;
                                                       Client_A->message_atop.respond_ = ResponseType::Refuse;
                                                       PreLayerInstance.pack_Message(ClieClient_Bnt_A); 
                                                }
                                                else {
                                                        LOG(Error) << "Can't Find Client A after B responsed." << endl;
                                                }
                                        }
                                        break;
                                }
                        }
                        break;
                }
                case SessionState::WaitForBoard: {
                }
        }

        return ;
}
