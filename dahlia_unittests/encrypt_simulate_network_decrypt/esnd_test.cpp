/** Copyright (C) December 26th 2014, Shane Betz **/
/** Notes
 *  This test will simulate the server/client communication;
 *  The server will listen for client requests for a 'hello',
 *  and the clients public key;
 *  The server will respond with their public key unencrypted,
 *  along with a response that is encrypted for the client,
 *  to decrypt.
 *  The client will decrypt them message;
 **/
#include <assert.h> //assert
#include <string.h> //memcpy
#include <cstring> //strcmp
#include <cstdlib> //malloc
#include <zmq.h> //major zmq functions
#include <sodium.h> //crypto
#include <jansson.h> //json


int main(void){
  printf("\n----------\nUnit Test Begin\n\n");
  //ASSERT sodium initalizes
  int test_sodium = sodium_init();
  assert(test_sodium != -1);
  
  //Report Version number
  int major, minor, patch;
  zmq_version (&major, &minor, &patch);
  printf("Current ZMQ version is %d.%d.%d\n", major, minor, patch);
  assert(major >= 4);
  
  /** Simulate a server for listening for requests */
  //ASSERT new server context
  void * test_server_context = zmq_ctx_new();
  assert(test_server_context != NULL);

  //ASSERT new server socket (request/reply socket type)
  void * test_server_socket = zmq_socket(test_server_context, ZMQ_REP);
  assert(test_server_socket != NULL);
  
  //ASSERT listening for requests
  int test_server_bind = zmq_bind(test_server_socket, "tcp://*:5555");
  assert(test_server_bind == 0);
  
  //ASSERT generation of servers keypair
  unsigned char test_server_publickey[crypto_box_PUBLICKEYBYTES];
  unsigned char test_server_secretkey[crypto_box_SECRETKEYBYTES];
  crypto_box_keypair(test_server_publickey, test_server_secretkey);
  
  /** Simulate a client making a request to the server **/
  //ASSERT new client context
  void * test_client_context = zmq_ctx_new();
  assert(test_client_context != NULL);
  
  //ASSERT new client socket
  void * test_client_socket = zmq_socket(test_client_context, ZMQ_REQ);
  assert(test_client_socket);
  
  //ASSERT 'connection' to simulated server
  int test_client_connect = zmq_connect(test_client_socket, "tcp://127.0.0.1:5555");
  assert(test_client_connect == 0);
  
  //ASSERT generation of clients keypair
  unsigned char test_client_publickey[crypto_box_PUBLICKEYBYTES];
  unsigned char test_client_secretkey[crypto_box_SECRETKEYBYTES];
  crypto_box_keypair(test_client_publickey, test_client_secretkey);
  
  //ASSERT construct a JSON format message that states 'hello' and the clients public key.
  json_t * test_client_root_msg = json_object(); //root of json
  json_t * test_client_req_type = json_string("hello"); //request type
  json_object_set(test_client_root_msg, "request", test_client_req_type);
  
  char test_client_publickey_char[crypto_box_PUBLICKEYBYTES]; //allocate memory for z85 encoding
  zmq_z85_encode(test_client_publickey_char, test_client_publickey, crypto_box_PUBLICKEYBYTES); //z85 encode
  json_t * test_client_req_publickey = json_string(test_client_publickey_char);
  json_object_set(test_client_root_msg, "identity", test_client_req_publickey);
  char * test_client_req_msg_char = json_dumps(test_client_root_msg, JSON_COMPACT);
  
  //ASSERT send JSON request to the simulated server
  zmq_msg_t test_client_hellomsg;
  int test_client_hellomsg_init_mem = zmq_msg_init_size(&test_client_hellomsg, strlen(test_client_req_msg_char));
  assert(test_client_hellomsg_init_mem == 0);
  memcpy(zmq_msg_data(&test_client_hellomsg), test_client_req_msg_char, strlen(test_client_req_msg_char));
  int test_client_hellomsg_send = zmq_msg_send(&test_client_hellomsg, test_client_socket, 0); 
  assert(test_client_hellomsg_send == strlen(test_client_req_msg_char));
  zmq_msg_close(&test_client_hellomsg);
  //printf("sent: %i\n", test_client_hellomsg_send);
  

  /** Simulate server receving message from client just sent **/
  //ASSERT recieve message
  zmq_msg_t test_server_msg_recv;
  int test_server_msg_recv_init = zmq_msg_init(&test_server_msg_recv);
  assert(test_server_msg_recv_init == 0);
  size_t test_server_msg_recv_bytes = zmq_msg_recv(&test_server_msg_recv, test_server_socket, 0);
  assert(test_server_msg_recv_bytes != -1);
  char * test_server_msg_char = (char *)malloc(test_server_msg_recv_bytes+1); // Sodium will secure the heap for us
  memcpy(test_server_msg_char, zmq_msg_data(&test_server_msg_recv), test_server_msg_recv_bytes); //convert zmq layer to c data type layer
  zmq_msg_close(&test_server_msg_recv);
  //test_server_msg_char[test_server_msg_recv_bytes+1] = '\0';
  //printf("receved: %s\n", test_server_msg_char);
  
  //ASSERT convert char to json object
  json_error_t test_server_msg_req_json_error;
  json_t * test_server_msg_req_json = json_loads(test_server_msg_char, JSON_DISABLE_EOF_CHECK, &test_server_msg_req_json_error);
  
  //ASSERT convert json objects to extracted char objects
  const char * test_server_msg_req_type = json_string_value(json_object_get(test_server_msg_req_json, "request"));
  const char * test_server_msg_identity = json_string_value(json_object_get(test_server_msg_req_json, "identity"));
  
  //ASSERT request type is hello
  assert(strcmp(test_server_msg_req_type, "hello") == 0);
  
  //ASSERT convert z85 encoded identity to z85 decoded identity
  uint8_t test_server_clients_identity[strlen(test_server_msg_identity)]; 
  zmq_z85_decode(test_server_clients_identity, (char*)test_server_msg_identity); //z85 decode
  
  //ASSERT generate json packet to store encrypted data for plaintext response
  json_t * test_server_root_msg = json_object();
  json_t * test_server_res_type = json_string("hi");
  json_object_set(test_server_root_msg, "response", test_server_res_type);
  
  //ASSERT generate json packet to be encrypted data for the plaintext json packet response
  json_t * test_server_root_msg_secret = json_object();
  char test_server_publickey_char[crypto_box_PUBLICKEYBYTES]; //allocate memory for z85 encoding
  zmq_z85_encode(test_server_publickey_char, test_server_publickey, crypto_box_PUBLICKEYBYTES); //z85 encode
  json_t * test_server_res_publickey = json_string(test_server_publickey_char);
  json_object_set(test_server_root_msg_secret, "identity", test_server_res_publickey);
  char * test_server_res_char = json_dumps(test_server_root_msg_secret, JSON_COMPACT);
  unsigned char * test_server_res_uchar = reinterpret_cast<unsigned char *>(test_server_res_char);
  size_t test_server_res_uchar_size = strlen(test_server_res_char);
  
  unsigned char test_server_res_nonce[crypto_box_NONCEBYTES];
  unsigned char test_server_res_ciphertext[(crypto_box_MACBYTES + test_server_res_uchar_size)];
  randombytes_buf(test_server_res_nonce, crypto_box_NONCEBYTES);
  crypto_box_easy(test_server_res_ciphertext, test_server_res_uchar, test_server_res_uchar_size, test_server_res_nonce, test_server_clients_identity, test_server_secretkey);
  size_t test_server_res_ciphertext_size = sizeof(test_server_res_ciphertext);
  //printf("original size: %lu\n", test_server_res_uchar_size);
  //printf("ciphertext size: %lu\n" , sizeof(test_server_res_ciphertext));
  //printf("ciphertext: %s\n", test_server_res_ciphertext);

  //ASSERT encode the secret text so it can be placed within JSON specifications
  size_t test_server_res_ciphertext_hex_size = test_server_res_ciphertext_size * 2 + 1;
  char * test_server_res_ciphertext_hex = (char*)malloc(test_server_res_ciphertext_hex_size);
  sodium_bin2hex(test_server_res_ciphertext_hex, test_server_res_ciphertext_hex_size, test_server_res_ciphertext, test_server_res_ciphertext_size);
  //printf("ciphertext hex encoded: %s\n", test_server_res_ciphertext_hex);
  
  //ASSERT join the encrypted payload into the plaintext json packet response payload
  json_t * test_server_res_payload = json_string(test_server_res_ciphertext_hex);
  json_object_set(test_server_root_msg, "payload", test_server_res_payload);
  
  //ASSERT convert plaintext json packet to char/string
  char * test_server_net_res_char = json_dumps(test_server_root_msg, JSON_COMPACT);
  
  //ASSERT send JSON formated message to client as a response to 'hello'
  zmq_msg_t test_server_response_msg;
  int test_server_response_msg_init_mem = zmq_msg_init_size(&test_server_response_msg, strlen(test_server_net_res_char));
  assert(test_server_response_msg_init_mem == 0);
  memcpy(zmq_msg_data(&test_server_response_msg), test_server_net_res_char, strlen(test_server_net_res_char));
  int test_server_response_msg_sent = zmq_msg_send(&test_server_response_msg, test_server_socket, 0); 
  assert(test_server_response_msg_sent == strlen(test_server_net_res_char));
  //printf("sent: %i\n", test_server_response_msg_sent);
  //printf("server response: %s\n", test_server_net_res_char);
  
  /** Simulate a client receiving the 'hi' response and the encrypted=>hex-encoded payload, then proceed to decode the hex then decrypt the payload **/
  //ASSERT recv message from server
  zmq_msg_t test_client_recv_himsg;
  int test_client_recv_himsg_init = zmq_msg_init(&test_client_recv_himsg);
  assert(test_client_recv_himsg_init == 0);
  int test_client_recv_himsg_bytes = zmq_msg_recv(&test_client_recv_himsg, test_client_socket, 0);
  assert(test_client_recv_himsg_bytes != -1);
  char * test_client_recv_himsg_char = (char *)malloc(test_client_recv_himsg_bytes); // Sodium will secure the heap for us
  memcpy(test_client_recv_himsg_char, zmq_msg_data(&test_client_recv_himsg), test_client_recv_himsg_bytes); //convert zmq layer to c data type layer
  test_client_recv_himsg_char[test_client_recv_himsg_bytes] = '\0';
  //printf("server response: %s\n", test_client_recv_himsg_char);

  //ASSERT convert char to JSON parseable object
  json_error_t test_client_himsg_json_error;
  json_t * test_client_himsg_json = json_loads(test_client_recv_himsg_char, JSON_DISABLE_EOF_CHECK, &test_client_himsg_json_error);
  
  //ASSERT extract response type and the servers (hex encoded)encrypted payload
  const char * test_client_himsg_res_type = json_string_value(json_object_get(test_client_himsg_json, "response"));
  const char * test_client_himsg_payload = json_string_value(json_object_get(test_client_himsg_json, "payload"));
  //printf("response type: %s\n", test_client_himsg_res_type);
  //printf("payload: %s\n", test_client_himsg_payload);
  
  //ASSERT request type is hello
  assert(strcmp(test_client_himsg_res_type, "hi") == 0);
  
  //ASSERT decode payload
  unsigned char test_client_himsg_payload_secret[strlen(test_client_himsg_payload)];
  size_t bin_len;
  const char * test_client_himsg_hex_end;
  sodium_hex2bin(test_client_himsg_payload_secret,  strlen(test_client_himsg_payload),  test_client_himsg_payload, strlen(test_client_himsg_payload), NULL, &bin_len, &test_client_himsg_hex_end);
  
  //ASSERT decrypt payload
  unsigned char decrypted[test_server_res_ciphertext_size];
  crypto_box_open_easy(decrypted, test_client_himsg_payload_secret, test_server_res_ciphertext_size, test_server_res_nonce, test_server_publickey, test_client_secretkey);
  printf("decrypted payload: %s\n" ,decrypted);
  //ASSERT [temp] decrypted
  /*unsigned char decrypted[test_server_res_ciphertext_size];
  if(crypto_box_open_easy(decrypted, test_server_res_ciphertext, test_server_res_ciphertext_size, test_server_res_nonce, test_server_publickey, test_client_secretkey) != 0){
      /* message from Bob received by Alice has been forged!
      printf("failed\n");
  }*/
  //printf("decrypted: %s\n" ,decrypted);
  
  
  
  //Test Succes (it made it this far)
  printf("\nUnit Test Success\n----------\n\n");
  return 0; 
}