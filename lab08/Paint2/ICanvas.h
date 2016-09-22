#pragma once

class IShape;

class ICanvas
{
public:
	virtual void InsertShape(std::shared_ptr<IShape> const & shape, boost::optional<size_t> pos) = 0;
	virtual void DeleteShape(std::shared_ptr<IShape> const & shape) = 0;
	virtual size_t GetShapeCount() const = 0;
	virtual std::shared_ptr<IShape> GetShapeAtIndex(size_t index) const = 0;

	using InsertShapeSignal = boost::signals2::signal<void(std::shared_ptr<IShape> const & shape, boost::optional<size_t> position)>;
	virtual void DoOnInsertShape(InsertShapeSignal::slot_type const & handler) = 0;

	using DeleteShapeSignal = boost::signals2::signal<void(std::shared_ptr<IShape> const & shape)>;
	virtual void DoOnDeleteShape(DeleteShapeSignal::slot_type const & handler) = 0;

	using ChangeShapeSignal = boost::signals2::signal<void(std::shared_ptr<IShape> const & shape)>;
	virtual void DoOnChangeShape(ChangeShapeSignal::slot_type const & handler) = 0;

	virtual ~ICanvas() = default;
};
