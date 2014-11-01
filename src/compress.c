#include "rb_tree.h"
#include "hm_tree.h"
#include "container.h"
#include "compress.h"
#include "debug.h"
#include "bits.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define MAX_CODE_NUM 256
#define INIT_BALL_SIZE 256

struct CompressNode {
	char val;
	int hit;
	struct HMNode hm_node;
};

struct CompressBall {
	char *data;
	unsigned int index;
	unsigned int bit_len;
	int lock_flag;
};

static inline struct CompressBall
makeBall()
{
	char *data = malloc(INIT_BALL_SIZE);
	assert(data != NULL);
	struct CompressBall new_ball = { data, 0, INIT_BALL_SIZE * BITS_PER_BYTE, 0 };
	return new_ball;
}

static inline void
chargeBall(struct CompressBall *ball)
{
	ball->bit_len *= 2;
	ball->data = realloc(ball->data, ball->bit_len / BITS_PER_BYTE);
	assert(ball->data != NULL);
}


static inline void
lockBall(struct CompressBall *ball)
{
	ball->lock_flag = 1;
}

static inline void
unlockBall(struct CompressBall *ball)
{
	ball->lock_flag = 0;
}

static inline int
isLock(struct CompressBall *ball)
{
	return ball->lock_flag;
}

static inline void
rollBack(struct CompressBall *ball)
{
	if(!isLock(ball))
		--ball->index;
}

static inline void
rollLeft(struct CompressBall *ball)
{
	if(isLock(ball))
		return;
	if(ball->index == ball->bit_len)
		chargeBall(ball);
	bits_clr(ball->data, ball->index);
	++ball->index;
}

static inline void
rollRight(struct CompressBall *ball)
{
	if(isLock(ball))
		return;
	if(ball->index == ball->bit_len)
		chargeBall(ball);
	bits_set(ball->data, ball->index);
	++ball->index;
}
	
static void
walkEncodeTree(struct HMNode *root, struct CompressBall *ball, char dest)
{
	if(isLock(ball))
		return;
	if(root->left == NULL && root->right == NULL) {
		if(container_of(root, struct CompressNode, hm_node)->val == dest)
			lockBall(ball);
		rollBack(ball);
		return;
	}
	if(root->left != NULL) {
		rollLeft(ball);
		walkEncodeTree(root->left, ball, dest);
	}
	if(root->right != NULL) {
		rollRight(ball);
		walkEncodeTree(root->right, ball, dest);
	}
	rollBack(ball);
}		

static int
hmTreeCmp(const void *p1, const void *p2) 
{
        struct HMNode **node1 = (struct HMNode **)p1;
        struct HMNode **node2 = (struct HMNode **)p2;
        return container_of(*node2, struct CompressNode, hm_node)->hit - 
                container_of(*node1, struct CompressNode, hm_node)->hit;
}

static struct HMNode *
hmTreeMerge(struct HMNode *t1, struct HMNode *t2)
{
        struct CompressNode *node = malloc(sizeof(struct CompressNode));
        assert(node != NULL);
        node->hit = container_of(t1, struct CompressNode, hm_node)->hit 
                + container_of(t2, struct CompressNode, hm_node)->hit;
        node->hm_node.left = t1; 
        node->hm_node.right = t2; 
        return &node->hm_node;
}

/* FOR DEBUG
static void
dumpHMTree(struct HMNode *tree)
{
        if(tree->left == NULL && tree->right == NULL) {
                printf("<%c %d>\n", container_of(tree, struct CompressNode, hm_node)->val, 
			container_of(tree, struct CompressNode, hm_node)->hit);
                printf("return\n");
                return;
        }
        if(tree->left != NULL) {
                printf("go left\n");
                dumpHMTree(tree->left);
        }
        if(tree->right != NULL) {
                printf("go right\n");
                dumpHMTree(tree->right);
        }
        printf("return\n");
}
*/

static void
destroyNode(struct HMNode *node)
{
        free(container_of(node, struct CompressNode, hm_node));
}

compress_t
makeCompressType(void *data, unsigned int size)
{
        void *ptr = malloc(size);
        assert(ptr != NULL);
        memcpy(ptr, data, size);    
        compress_t new_compress_session = { ptr, size * BITS_PER_BYTE };
        return new_compress_session;
}

void
destroyCompressType(compress_t session)
{
       	if(session.data != NULL)
                free(session.data);
	destroyHMTree(session.encode_tree, destroyNode);
}

compress_t
compressEncode(compress_t session)
{
        int hits[MAX_CODE_NUM] = { 0 };
        unsigned char *ptr = session.data;
        for(int i = 0; i < session.bit_size / BITS_PER_BYTE; i++)
                ++hits[(int)ptr[i]];
	int tree_num = 0;
	for(int i = 0; i < MAX_CODE_NUM; i++) {
		if(hits[i] != 0)
			tree_num++;
	}
	struct HMNode **nodeArray = malloc(tree_num * sizeof(struct HMNode *));
	assert(nodeArray != NULL);
	int index = 0;
	for(int i = 0; i < tree_num; i++) {
		struct CompressNode *node = malloc(sizeof(struct CompressNode));
		assert(node != NULL);
		memset(node, 0, sizeof(struct CompressNode));
		while(hits[index] == 0)
			++index;
		node->val = index;
		node->hit = hits[index++];
		nodeArray[i] = &node->hm_node;
	}
        compress_t new_session;
	HMSeed hm_seed = makeHMSeed(nodeArray, tree_num, hmTreeCmp, hmTreeMerge);
	new_session.encode_tree = makeHMTree(hm_seed);
	free(nodeArray);
	struct CompressBall ball = makeBall();
	for(int i = 0; i < session.bit_size / BITS_PER_BYTE; i++) {
		walkEncodeTree(getHMRoot(new_session.encode_tree), &ball, ((char *)session.data)[i]);
		assert(isLock(&ball));
		unlockBall(&ball);
	}
	new_session.data = ball.data;
	new_session.bit_size = ball.index;
        return new_session;
}

compress_t
compressDecode(compress_t session)
{
	int data_size = INIT_BALL_SIZE;
	char *data = malloc(data_size);
	assert(data != NULL);
	int index = 0;
	struct HMNode *node = getHMRoot(session.encode_tree);
	for(int i = 0; i < session.bit_size; i++) {
		if(index == data_size) {
			data_size *= 2;
			data = realloc(data, data_size);
			assert(data != NULL);
		}
		if(bits_test(session.data, i)) {
			assert(node->right != NULL);
			node = node->right;
		} else {
			assert(node->left != NULL);
			node = node->left;
		}
		if(node->left == NULL && node->right == NULL) {
			data[index++] = container_of(node, struct CompressNode, hm_node)->val;
			node = getHMRoot(session.encode_tree);
		}
	}
	compress_t new_session = { data, index * BITS_PER_BYTE };
	return new_session;
}

static void
dumpEncodeTree(FILE *file, struct HMNode *tree, short num)
{
	if(tree->left == NULL && tree->right == NULL) {
		fwrite(&container_of(tree, struct CompressNode, hm_node)->val, sizeof(char), 1, file);
		fwrite(&num, sizeof(short), 1, file);
		return;
	}
	if(tree->left != NULL)
		dumpEncodeTree(file, tree->left, num * 2);
	if(tree->right != NULL);
		dumpEncodeTree(file, tree->right, num * 2 + 1);
}
			
static void
dumpCompressType(FILE *file, compress_t session)
{
	if(getHMRoot(session.encode_tree) == NULL) {
		fwrite(session.data, session.bit_size / BITS_PER_BYTE, 1, file);
	} else {
		fwrite(&session.bit_size, sizeof(session.bit_size), 1, file);
		fwrite(&session.bit_size, sizeof(session.bit_size), 1, file);
		dumpEncodeTree(file, getHMRoot(session.encode_tree), 1);
		int pos = ftell(file);
		fwrite(session.data, session.bit_size / BITS_PER_BYTE + 1, 1, file);
		fseek(file, sizeof(int), SEEK_SET);
		fwrite(&pos, sizeof(int), 1, file);
	}
}
	
void
compressEncodeFile(const char *in_file, const char *out_file)
{
	FILE *in_f = fopen(in_file, "rb");
	assert(in_f != NULL);
	FILE *out_f = fopen(out_file, "wb");
	assert(out_f != NULL);
	int data_size = INIT_BALL_SIZE;
	int index = 0;
	char *data = malloc(data_size);
	assert(data != NULL);
	char ch;
	while(1) {
		ch = getc(in_f);
		if(feof(in_f))
			break;
		if(index == data_size) {
			data_size *= 2;
			data = realloc(data, data_size);
			assert(data != NULL);
		}
		data[index++] = ch;
	}
	compress_t encode_session = makeCompressType(data, index);
	free(data);
	compress_t result_session = compressEncode(encode_session);
	destroyCompressType(encode_session);
	dumpCompressType(out_f, result_session);
	destroyCompressType(result_session);
	fclose(in_f);
	fclose(out_f);
}

static inline int
findPos(char *buf, unsigned int size, short num, char *val)
{
	short index;
	for(int i = 0; i < size; i += sizeof(short) + sizeof(char)) {
		memcpy(val, &buf[i], sizeof(char));
		memcpy(&index, &buf[i+sizeof(char)], sizeof(short));
		if(index == num)
			return 1;
	}
	return 0;
}

static void
loadEncodeTree(char *buf, unsigned int size, struct HMNode **hm_node_ptr, short num)
{
	struct CompressNode *node = malloc(sizeof(struct CompressNode));
	assert(node != NULL);
	memset(node, 0, sizeof(struct CompressNode));
	*hm_node_ptr = &node->hm_node;
	if(findPos(buf, size, num, &node->val)) {
		return;
	}
	loadEncodeTree(buf, size, &node->hm_node.left, num * 2);
	loadEncodeTree(buf, size, &node->hm_node.right, num * 2 + 1);
}

void
compressDecodeFile(const char *in_file, const char *out_file)
{
	FILE *in_f = fopen(in_file, "rb");
	assert(in_f != NULL);
	FILE *out_f = fopen(out_file, "wb");
	assert(out_f != NULL);
	compress_t decode_session;
	fread(&decode_session.bit_size, sizeof(decode_session.bit_size), 1, in_f);
	int pos = 0;
	fread(&pos, sizeof(pos), 1, in_f);
	int buf_size = pos - ftell(in_f);
	char *buf = malloc(buf_size);
	assert(buf != NULL);
	fread(buf, buf_size, 1, in_f);
	loadEncodeTree(buf, buf_size, &decode_session.encode_tree.root, 1);
//	dumpHMTree(decode_session.encode_tree.root);
	int data_size = INIT_BALL_SIZE;
	int index = 0;
	char *data = malloc(data_size);
	assert(data != NULL);
	char ch;
	while(1) {
		ch = getc(in_f);
		if(feof(in_f))
			break;
                if(index == data_size) {
                         data_size *= 2;
                         data = realloc(data, data_size);
                         assert(data != NULL);
                 }
		data[index++] = ch;	
	}
	decode_session.data = data;
	compress_t result_session = compressDecode(decode_session);
	dumpCompressType(out_f, result_session);
	destroyCompressType(decode_session);
	destroyCompressType(result_session);
	fclose(in_f);
	fclose(out_f);
}		
