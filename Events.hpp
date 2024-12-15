#pragma once

#include "EventParams.hpp"
#include "TimeFormat.hpp"
#include "Event.hpp"

class Events
{
public:
	struct Node {
		Event value;
		Node* next = nullptr;
	};

	~Events() {
		last = nullptr;
		for (Node* node = GetHead(); node; node = node->next) {
			PopFront();
		}
	}

	void InsertAfter(Node* node, const Event& value)
	{
		if (node == nullptr) {
			Node* newHead = new Node;
			newHead->value = value;
			newHead->next = head;
			head = newHead;
			last = newHead;
		} else {
			Node* newNode = new Node;
			newNode->value = value;
			newNode->next = node->next;
			node->next = newNode;
			if (node->next == nullptr) {
				last = node;
			}
		}
	}

	void PopFront() {
		if (head != nullptr) {
			Node* currentHead = head;
			head = head->next;
			delete currentHead;
		}
	}

	Node* GetHead() { return head; }
	Node* GetLast() { return last; }
	const Node* GetHead() const { return head; }
	const Node* GetLast() const { return last; }

private:
	Node* head = nullptr;
	Node* last = nullptr;
};
