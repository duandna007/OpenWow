#pragma once

class ByteBuffer
{
public:
	 ByteBuffer();
	 ByteBuffer(const ByteBuffer& _other);

	 ByteBuffer(uint64_t _size);
	 ByteBuffer(uint8_t* _data, uint64_t _size);
	 ~ByteBuffer();

	//

	 void Allocate(uint64_t _size);
	 void CopyData(uint8_t* _data, uint64_t _size);
	 void Init(uint8_t* _dataPtr, uint64_t _size);

	//

	 const string ReadLine();
	 const void ReadBytes(void* _destination, uint64_t _size);

	//

	inline uint64_t GetSize() const { return bufferSize; }
	inline uint64_t GetPos() const { return position; }
	inline const uint8_t* GetData() const { return data; }
	inline uint8_t* GetDataFromCurrent() { return data + position; }
	inline bool IsEof() const { return isEof; }

	 void Seek(uint64_t _bufferOffsetAbsolute);
	 void SeekRelative(uint64_t _bufferOffsetRelative);

protected:
	bool isFilled;
	bool isOnlyPointerToData;

	bool isEof;
	bool allocated;
	uint8_t* data;
	uint64_t position;
	uint64_t bufferSize;
};