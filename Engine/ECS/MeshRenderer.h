namespace Burst {
	struct MeshRenderer : public ECS::Component {
		Mesh* mesh;
		Material* material;

		RenderObject renderObject;

		MeshRenderer(Mesh* _mesh, Material* _material) {
			this->mesh = _mesh;
			this->material = _material;

			this->renderObject = RenderObject(this->mesh, this->material->shader);
		}

		virtual void Render() {
			this->material->Bind();
			this->material->shader->SendMatrix4("modelMatrix", this->parent->transform.GetModelMatrix());
			renderObject.Render();
		}
	};
}